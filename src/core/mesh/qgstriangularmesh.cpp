/***************************************************************************
                         qgstriangularmesh.cpp
                         ---------------------
    begin                : April 2018
    copyright            : (C) 2018 by Peter Petrik
    email                : zilolv at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <memory>
#include <QList>
#include "qgspolygon.h"
#include "qgslinestring.h"
#include "qgstriangularmesh.h"
#include "qgsrendercontext.h"
#include "qgscoordinatetransform.h"
#include "qgsfeatureid.h"
#include "qgsgeometry.h"
#include "qgsrectangle.h"
#include "qgsfeatureiterator.h"
#include "qgslogger.h"

///@cond PRIVATE

QgsMeshFeatureIterator::QgsMeshFeatureIterator( QgsMesh *mesh )
  : QgsAbstractFeatureIterator( QgsFeatureRequest() )
  , mMesh( mesh )
{}

QgsMeshFeatureIterator::~QgsMeshFeatureIterator() = default;

bool QgsMeshFeatureIterator::rewind()
{
  it = 0;
  return true;
}
bool QgsMeshFeatureIterator::close()
{
  mMesh = nullptr;
  return true;
}

bool QgsMeshFeatureIterator::fetchFeature( QgsFeature &f )
{
  if ( !mMesh || mMesh->faces.size() <= it )
    return false;

  const QgsMeshFace &face = mMesh->faces.at( it ) ;
  QgsGeometry geom = QgsMeshUtils::toGeometry( face, mMesh->vertices );
  f.setGeometry( geom );
  f.setId( it );
  ++it;
  return true;
}


///@endcond

static void ENP_centroid_step( const QPolygonF &pX, double &cx, double &cy, double &signedArea, int i, int i1 )
{
  double x0 = 0.0; // Current vertex X
  double y0 = 0.0; // Current vertex Y
  double x1 = 0.0; // Next vertex X
  double y1 = 0.0; // Next vertex Y
  double a = 0.0;  // Partial signed area

  x0 = pX[i].x();
  y0 = pX[i].y();
  x1 = pX[i1].x();
  y1 = pX[i1].y();
  a = x0 * y1 - x1 * y0;
  signedArea += a;
  cx += ( x0 + x1 ) * a;
  cy += ( y0 + y1 ) * a;
}

static void ENP_centroid( const QPolygonF &pX, double &cx, double &cy )
{
  // http://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon/2792459#2792459
  cx = 0;
  cy = 0;

  if ( pX.isEmpty() )
    return;

  double signedArea = 0.0;

  // For all vertices except last
  int i = 0;
  for ( ; i < pX.size() - 1; ++i )
  {
    ENP_centroid_step( pX, cx, cy, signedArea, i, i + 1 );
  }
  // Do last vertex separately to avoid performing an expensive
  // modulus operation in each iteration.
  ENP_centroid_step( pX, cx, cy, signedArea, i, 0 );

  signedArea *= 0.5;
  cx /= ( 6.0 * signedArea );
  cy /= ( 6.0 * signedArea );
}

void QgsTriangularMesh::triangulate( const QgsMeshFace &face,
                                     int nativeIndex,
                                     const QVector<int> &nativeVerticesToVertices )
{
  int vertexCount = face.size();
  if ( vertexCount < 3 )
    return;

  while ( vertexCount > 3 )
  {
    // clip one ear from last 2 and first vertex
    const int vertexIndex1 = nativeVerticesToVertices[face[vertexCount - 2]];
    const int vertexIndex2 = nativeVerticesToVertices[face[vertexCount - 1]];
    const int vertexIndex3 = nativeVerticesToVertices[face[0]];
    const QgsMeshFace ear = { vertexIndex1, vertexIndex2, vertexIndex3 };
    mTriangularMesh.faces.push_back( ear );
    mTrianglesToNativeFaces.push_back( nativeIndex );

    --vertexCount;
  }

  const int vertexIndex1 = nativeVerticesToVertices[face[1]];
  const int vertexIndex2 = nativeVerticesToVertices[face[2]];
  const int vertexIndex3 = nativeVerticesToVertices[face[0]];
  const QgsMeshFace triangle = { vertexIndex1, vertexIndex2, vertexIndex3 };
  mTriangularMesh.faces.push_back( triangle );
  mTrianglesToNativeFaces.push_back( nativeIndex );
}

QVector<int> QgsTriangularMesh::transformVertices( QgsMesh *nativeMesh )
{
  QVector<int> nativeVerticesToVertices( nativeMesh->vertices.size() );

  for ( int i = 0; i < nativeMesh->vertices.size(); ++i )
  {
    const QgsMeshVertex &vertex = nativeMesh->vertices.at( i );
    if ( mCoordinateTransform.isValid() )
    {
      try
      {
        QgsPointXY mapPoint = mCoordinateTransform.transform( QgsPointXY( vertex.x(), vertex.y() ) );
        QgsMeshVertex mapVertex( mapPoint );
        mapVertex.addZValue( vertex.z() );
        mapVertex.setM( vertex.m() );

        nativeVerticesToVertices[i] = mVerticesToNativeVertices.size();
        mTriangularMesh.vertices.push_back( mapVertex );
        mVerticesToNativeVertices.push_back( i );
      }
      catch ( QgsCsException &cse )
      {
        Q_UNUSED( cse );
        QgsDebugMsg( QStringLiteral( "Caught CRS exception %1" ).arg( cse.what() ) );
        nativeVerticesToVertices[i] = -1; // invalid transformation
      }
    }
    else
    {
      nativeVerticesToVertices[i] = mVerticesToNativeVertices.size();
      mTriangularMesh.vertices.push_back( vertex );
      mVerticesToNativeVertices.push_back( i );
    }
  }

  return nativeVerticesToVertices;
}

void QgsTriangularMesh::calculateCentroid(
  const QgsMeshFace &face,
  int nativeIndex,
  const QVector<int> &nativeVerticesToVertices )
{
  QVector<QPointF> points;
  points.reserve( face.size() );
  for ( int j = 0; j < face.size(); ++j )
  {
    const int nativeVertexIndex = face[j];
    const int vertexIndex = nativeVerticesToVertices[nativeVertexIndex];
    if ( vertexIndex < 0 )
    {
      // invalid vertex => no centroid
      mNativeMeshFaceCentroids[nativeIndex] = QgsMeshVertex( std::numeric_limits<double>::quiet_NaN(),
                                                             std::numeric_limits<double>::quiet_NaN() );
      return;
    }
    const QgsMeshVertex &vertex = mTriangularMesh.vertices[vertexIndex]; // we need projected vertices
    points.push_back( vertex.toQPointF() );
  }
  QPolygonF poly( points );
  double cx, cy;
  ENP_centroid( poly, cx, cy );
  mNativeMeshFaceCentroids[nativeIndex] = QgsMeshVertex( cx, cy );
}

void QgsTriangularMesh::update( QgsMesh *nativeMesh, QgsRenderContext *context )
{
  Q_ASSERT( nativeMesh );
  Q_ASSERT( context );

  // FIND OUT IF UPDATE IS NEEDED
  if ( !mTriangularMesh.vertices.empty() &&
       !mTriangularMesh.faces.empty() &&
       mCoordinateTransform.sourceCrs() == context->coordinateTransform().sourceCrs() &&
       mCoordinateTransform.destinationCrs() == context->coordinateTransform().destinationCrs() )
    return;

  // CLEAN-UP
  mTriangularMesh.vertices.clear();
  mTriangularMesh.faces.clear();
  mTrianglesToNativeFaces.clear();
  mNativeMeshFaceCentroids.clear();
  mVerticesToNativeVertices.clear();

  // TRANSFORM NATIVE VERTICES
  mCoordinateTransform = context->coordinateTransform();
  const QVector<int> nativeVerticesToVertices = transformVertices( nativeMesh );

  // CREATE TRIANGULAR MESH
  for ( int i = 0; i < nativeMesh->faces.size(); ++i )
  {
    const QgsMeshFace &face = nativeMesh->faces.at( i ) ;
    triangulate( face, i, nativeVerticesToVertices );
  }

  // CALCULATE CENTROIDS
  mNativeMeshFaceCentroids.resize( nativeMesh->faces.size() );
  for ( int i = 0; i < nativeMesh->faces.size(); ++i )
  {
    const QgsMeshFace &face = nativeMesh->faces.at( i ) ;
    calculateCentroid( face, i, nativeVerticesToVertices );
  }

  // CALCULATE SPATIAL INDEX
  mSpatialIndex = QgsSpatialIndex( new QgsMeshFeatureIterator( &mTriangularMesh ) );
}

const QVector<QgsMeshVertex> &QgsTriangularMesh::vertices() const
{
  return mTriangularMesh.vertices;
}

const QVector<QgsMeshFace> &QgsTriangularMesh::triangles() const
{
  return mTriangularMesh.faces;
}

const QVector<QgsMeshVertex> &QgsTriangularMesh::centroids() const
{
  return mNativeMeshFaceCentroids;
}

const QVector<int> &QgsTriangularMesh::trianglesToNativeFaces() const
{
  return mTrianglesToNativeFaces;
}

const QVector<int> &QgsTriangularMesh::verticesToNativeVertices() const
{
  return mVerticesToNativeVertices;
}

int QgsTriangularMesh::faceIndexForPoint( const QgsPointXY &point ) const
{
  const QList<QgsFeatureId> faceIndexes = mSpatialIndex.intersects( QgsRectangle( point, point ) );
  for ( const QgsFeatureId fid : faceIndexes )
  {
    int faceIndex = static_cast<int>( fid );
    const QgsMeshFace &face = mTriangularMesh.faces.at( faceIndex );
    const QgsGeometry geom = QgsMeshUtils::toGeometry( face, mTriangularMesh.vertices );
    if ( geom.contains( &point ) )
      return faceIndex;
  }
  return -1;
}

QList<int> QgsTriangularMesh::faceIndexesForRectangle( const QgsRectangle &rectangle ) const
{
  const QList<QgsFeatureId> faceIndexes = mSpatialIndex.intersects( rectangle );
  QList<int> indexes;
  for ( const QgsFeatureId fid : faceIndexes )
  {
    int faceIndex = static_cast<int>( fid );
    indexes.append( faceIndex );
  }
  return indexes;
}

std::unique_ptr< QgsPolygon > QgsMeshUtils::toPolygon( const QgsMeshFace &face, const QVector<QgsMeshVertex> &vertices )
{
  QVector<QgsPoint> ring;
  for ( int j = 0; j < face.size(); ++j )
  {
    int vertexId = face[j];
    Q_ASSERT( vertexId < vertices.size() );
    const QgsPoint &vertex = vertices[vertexId];
    ring.append( vertex );
  }
  std::unique_ptr< QgsPolygon > polygon = qgis::make_unique< QgsPolygon >();
  polygon->setExteriorRing( new QgsLineString( ring ) );
  return polygon;
}

QgsGeometry QgsMeshUtils::toGeometry( const QgsMeshFace &face, const QVector<QgsMeshVertex> &vertices )
{
  return QgsGeometry( QgsMeshUtils::toPolygon( face, vertices ) );
}

QList<int> QgsMeshUtils::nativeFacesFromTriangles( const QList<int> &triangleIndexes, const QVector<int> &trianglesToNativeFaces )
{
  QSet<int> nativeFaces;
  for ( const int triangleIndex : triangleIndexes )
  {
    const int nativeIndex = trianglesToNativeFaces[triangleIndex];
    nativeFaces.insert( nativeIndex );
  }
  return nativeFaces.toList();
}
