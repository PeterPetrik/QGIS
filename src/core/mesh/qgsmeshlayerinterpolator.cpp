/***************************************************************************
                         qgsmeshlayerinterpolator.cpp
                         ----------------------------
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

///@cond PRIVATE

#include "qgsmeshlayerinterpolator.h"

#include "qgsrasterinterface.h"
#include <QVector2D>
#include "qgsmaptopixel.h"

static void bbox2rect(
  const QgsMapToPixel &mtp,
  const QSize &outputSize,
  const QgsRectangle &bbox,
  int &leftLim, int &rightLim, int &topLim, int &bottomLim )
{
  QgsPointXY ll = mtp.transform( bbox.xMinimum(), bbox.yMinimum() );
  QgsPointXY ur = mtp.transform( bbox.xMaximum(), bbox.yMaximum() );
  topLim = std::max( int( ur.y() ), 0 );
  bottomLim = std::min( int( ll.y() ), outputSize.height() - 1 );
  leftLim = std::max( int( ll.x() ), 0 );
  rightLim = std::min( int( ur.x() ), outputSize.width() - 1 );
}

static void lam_tol( double &lam )
{
  const static double eps = 1e-6;
  if ( ( lam < 0.0 ) && ( lam > -eps ) )
  {
    lam = 0.0;
  }
}

static bool E3T_physicalToBarycentric( QPointF pA, QPointF pB, QPointF pC, QPointF pP, double &lam1, double &lam2, double &lam3 )
{
  if ( pA == pB || pA == pC || pB == pC )
    return false; // this is not a valid triangle!

  // Compute vectors
  QVector2D v0( pC - pA );
  QVector2D v1( pB - pA );
  QVector2D v2( pP - pA );

  // Compute dot products
  double dot00 = QVector2D::dotProduct( v0, v0 );
  double dot01 = QVector2D::dotProduct( v0, v1 );
  double dot02 = QVector2D::dotProduct( v0, v2 );
  double dot11 = QVector2D::dotProduct( v1, v1 );
  double dot12 = QVector2D::dotProduct( v1, v2 );

  // Compute barycentric coordinates
  double invDenom = 1.0 / ( dot00 * dot11 - dot01 * dot01 );
  lam1 = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
  lam2 = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;
  lam3 = 1.0 - lam1 - lam2;

  // Apply some tolerance to lam so we can detect correctly border points
  lam_tol( lam1 );
  lam_tol( lam2 );
  lam_tol( lam3 );

  // Return if POI is outside triangle
  if ( ( lam1 < 0 ) || ( lam2 < 0 ) || ( lam3 < 0 ) )
  {
    return false;
  }

  return true;
}


double interpolateFromVerticesData( const QPointF &p1, const QPointF &p2, const QPointF &p3, double val1, double val2, double val3, const QPointF &pt )
{
  double lam1, lam2, lam3;
  if ( !E3T_physicalToBarycentric( p1, p2, p3, pt, lam1, lam2, lam3 ) )
    return std::numeric_limits<double>::quiet_NaN();

  return lam1 * val3 + lam2 * val2 + lam3 * val1;
}

double interpolateFromFacesData( const QPointF &p1, const QPointF &p2, const QPointF &p3, double val, const QPointF &pt )
{
  double lam1, lam2, lam3;
  if ( !E3T_physicalToBarycentric( p1, p2, p3, pt, lam1, lam2, lam3 ) )
    return std::numeric_limits<double>::quiet_NaN();

  return val;
}

QgsMeshLayerInterpolator::QgsMeshLayerInterpolator( const QgsTriangularMesh &m,
    const QVector<double> &datasetValues, bool dataIsOnVertices,
    const QgsRenderContext &context,
    const QSize &size )
  : mTriangularMesh( m ),
    mDatasetValues( datasetValues ),
    mContext( context ),
    mDataOnVertices( dataIsOnVertices ),
    mOutputSize( size )
{
}

QgsMeshLayerInterpolator::~QgsMeshLayerInterpolator() = default;

QgsRasterInterface *QgsMeshLayerInterpolator::clone() const
{
  return nullptr;  // we should not need this (hopefully)
}

Qgis::DataType QgsMeshLayerInterpolator::dataType( int ) const
{
  return Qgis::Float64;
}

int QgsMeshLayerInterpolator::bandCount() const
{
  return 1;
}

QgsRasterBlock *QgsMeshLayerInterpolator::block( int, const QgsRectangle &extent, int width, int height, QgsRasterBlockFeedback *feedback )
{
  QgsRasterBlock *b = new QgsRasterBlock( Qgis::Float64, width, height );
  b->setIsNoData();  // assume initially that all values are unset
  double *data = reinterpret_cast<double *>( b->bits() );

  const QVector<QgsMeshFace> &triangels = mTriangularMesh.triangles();
  const QVector<QgsMeshVertex> &vertices = mTriangularMesh.vertices();

  // currently expecting that triangulation does not add any new extra vertices on the way
  if ( mDataOnVertices )
    Q_ASSERT( mDatasetValues.count() == mTriangularMesh.vertices().count() );

  for ( int i = 0; i < triangels.size(); ++i )
  {
    if ( feedback && feedback->isCanceled() )
      break;

    const QgsMeshFace &face = triangels[i];

    const int v1 = face[0], v2 = face[1], v3 = face[2];
    const QgsPoint p1 = vertices[v1], p2 = vertices[v2], p3 = vertices[v3];

    QgsRectangle bbox;
    bbox.combineExtentWith( p1.x(), p1.y() );
    bbox.combineExtentWith( p2.x(), p2.y() );
    bbox.combineExtentWith( p3.x(), p3.y() );
    if ( !extent.intersects( bbox ) )
      continue;

    // Get the BBox of the element in pixels
    int topLim, bottomLim, leftLim, rightLim;
    bbox2rect( mContext.mapToPixel(), mOutputSize, bbox, leftLim, rightLim, topLim, bottomLim );

    // interpolate in the bounding box of the face
    for ( int j = topLim; j <= bottomLim; j++ )
    {
      double *line = data + ( j * width );
      for ( int k = leftLim; k <= rightLim; k++ )
      {
        double val;
        QPointF p = mContext.mapToPixel().toMapCoordinates( k, j ).toQPointF();
        if ( mDataOnVertices )
          val = interpolateFromVerticesData(
                  QPointF( p1.x(), p1.y() ),
                  QPointF( p2.x(), p2.y() ),
                  QPointF( p3.x(), p3.y() ),
                  mDatasetValues[v1],
                  mDatasetValues[v2],
                  mDatasetValues[v3],
                  p );
        else
        {
          int face = mTriangularMesh.trianglesToNativeFaces()[i];
          val = interpolateFromFacesData(
                  QPointF( p1.x(), p1.y() ),
                  QPointF( p2.x(), p2.y() ),
                  QPointF( p3.x(), p3.y() ),
                  mDatasetValues[face],
                  p
                );
        }

        if ( !qIsNaN( val ) )
        {
          line[k] = val;
          b->setIsData( j, k );
        }
      }
    }

  }

  return b;
}

///@endcond
