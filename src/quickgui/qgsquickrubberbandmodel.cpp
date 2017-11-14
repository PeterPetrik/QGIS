/***************************************************************************
 qgsquicksrubberbandmodel.cpp
  --------------------------------------
  Date                 : 10.6.2016
  Copyright            : (C) 2016 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickrubberbandmodel.h"
#include <qgsvectorlayer.h>

QgsQuickRubberbandModel::QgsQuickRubberbandModel( QObject* parent )
  : QObject( parent )
  , mCurrentCoordinateIndex( 0 )
  , mGeometryType( QgsWkbTypes::LineGeometry )
  , mLayer( nullptr )
{
  mPointList.insert( 0, QgsPoint() );
}

int QgsQuickRubberbandModel::vertexCount() const
{
  return mPointList.size();
}

bool QgsQuickRubberbandModel::isEmpty() const
{
  return mPointList.isEmpty();
}

QVector<QgsPoint> QgsQuickRubberbandModel::vertices() const
{
  return mPointList;
}

QVector<QgsPoint> QgsQuickRubberbandModel::flatVertices() const
{
  QVector<QgsPoint> points;
  Q_FOREACH( const QgsPoint& pt, mPointList )
  {
    points << QgsPoint( pt );
  }

  return points;
}

QgsPointSequence QgsQuickRubberbandModel::pointSequence( const QgsCoordinateReferenceSystem& crs ) const
{
  QgsPointSequence sequence;

  QgsCoordinateTransform ct( mCrs, crs );

  Q_FOREACH( const QgsPoint& pt, mPointList )
  {
    QgsPoint p2( ct.transform( pt.x(), pt.y() ) );
    p2.setZ( pt.z() );
    sequence.append( p2 );
  }

  return sequence;
}

QList<QgsPointXY> QgsQuickRubberbandModel::flatPointSequence( const QgsCoordinateReferenceSystem& crs ) const
{
  QList<QgsPointXY> sequence;

  QgsCoordinateTransform ct( mCrs, crs );

  Q_FOREACH( const QgsPoint& pt, mPointList )
  {
    sequence.append( ct.transform( pt.x(), pt.y() ) );
  }

  return sequence;
}

void QgsQuickRubberbandModel::setVertex( int index, QgsPoint coordinate )
{
  if ( mPointList.at( index ) != coordinate )
  {
    mPointList.replace( index, coordinate );
    emit vertexChanged( index );
  }
}

void QgsQuickRubberbandModel::insertVertices( int index, int count )
{
  for ( int i = 0; i < count; ++i )
  {
    mPointList.insert( index, currentCoordinate() );
  }

  emit verticesInserted( index, count );
  emit vertexCountChanged();
}

void QgsQuickRubberbandModel::removeVertices( int index, int count )
{
  if ( mPointList.size() == 1 )
    return;

  mPointList.remove( index, count );
  emit verticesRemoved( index, count );
  emit vertexCountChanged();

  if ( mCurrentCoordinateIndex >= mPointList.size() )
  {
    setCurrentCoordinateIndex( mPointList.size() - 1 );
    emit currentCoordinateChanged();
  }
}

int QgsQuickRubberbandModel::currentCoordinateIndex() const
{
  return mCurrentCoordinateIndex;
}

void QgsQuickRubberbandModel::setCurrentCoordinateIndex( int currentCoordinateIndex )
{
  if ( currentCoordinateIndex == mCurrentCoordinateIndex )
    return;

  mCurrentCoordinateIndex = currentCoordinateIndex;
  emit currentCoordinateIndexChanged();
  emit currentCoordinateChanged();
}

QgsPoint QgsQuickRubberbandModel::currentPoint( const QgsCoordinateReferenceSystem& crs ) const
{
  QgsCoordinateTransform ct( mCrs, crs );

  QgsPoint currentPt = mPointList.at( mCurrentCoordinateIndex );
  double x = currentPt.x();
  double y = currentPt.y();
  double z = QgsWkbTypes::hasZ( currentPt.wkbType() ) ? currentPt.z() : 0;

  ct.transformInPlace( x, y, z );

  QgsPoint resultPt( x, y );
  if ( QgsWkbTypes::hasZ( currentPt.wkbType() ) )
    resultPt.addZValue( z );

  return resultPt;
}

QgsPoint QgsQuickRubberbandModel::currentCoordinate() const
{
  return mPointList.at( mCurrentCoordinateIndex );
}

void QgsQuickRubberbandModel::setCurrentCoordinate( const QgsPoint& currentCoordinate )
{
  if ( mPointList.at( mCurrentCoordinateIndex ) == currentCoordinate )
    return;

  mPointList.replace( mCurrentCoordinateIndex, currentCoordinate );
  emit currentCoordinateChanged();
  emit vertexChanged( mCurrentCoordinateIndex );
}

void QgsQuickRubberbandModel::addVertex()
{
  // Avoid double vertices accidentally
  if ( mPointList.size() > 1 && *( mPointList.end() - 1 ) == *( mPointList.end() - 2 ) )
    return;

  insertVertices( mCurrentCoordinateIndex + 1, 1 );
  setCurrentCoordinateIndex( mCurrentCoordinateIndex + 1 );
}

void QgsQuickRubberbandModel::removeVertex()
{
  setCurrentCoordinateIndex( mCurrentCoordinateIndex - 1 );
  removeVertices( mCurrentCoordinateIndex + 1, 1 );
}

void QgsQuickRubberbandModel::reset()
{
  removeVertices( 0, mPointList.size() - 1 );
}

QgsWkbTypes::GeometryType QgsQuickRubberbandModel::geometryType() const
{
  return mGeometryType;
}

void QgsQuickRubberbandModel::setGeometryType( const QgsWkbTypes::GeometryType& geometryType )
{
  if ( mGeometryType == geometryType )
    return;

  mGeometryType = geometryType;
  emit geometryTypeChanged();
}

QgsCoordinateReferenceSystem QgsQuickRubberbandModel::crs() const
{
  return mCrs;
}

void QgsQuickRubberbandModel::setCrs( const QgsCoordinateReferenceSystem& crs )
{
  if ( crs == mCrs )
    return;

  mCrs = crs;
  emit crsChanged();
}

QgsVectorLayer* QgsQuickRubberbandModel::vectorLayer() const
{
  return mLayer;
}

void QgsQuickRubberbandModel::setVectorLayer( QgsVectorLayer* layer )
{
  if ( layer == mLayer )
    return;

  mLayer = layer;

  if ( mLayer )
    setGeometryType( mLayer->geometryType() );

  emit vectorLayerChanged();
}
