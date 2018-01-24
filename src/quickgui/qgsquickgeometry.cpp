/***************************************************************************
 qgsquickgeometry.cpp
  --------------------------------------
  Date                 : Nov 2017
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

#include "qgslinestring.h"
#include "qgspoint.h"
#include "qgspolygon.h"
#include "qgsvectorlayer.h"

#include "qgsquickgeometry.h"

QgsQuickGeometry::QgsQuickGeometry( QObject *parent )
  : QObject( parent )
{
}

QgsGeometry QgsQuickGeometry::asQgsGeometry() const
{
  QgsAbstractGeometry *geom = nullptr;

  if ( !mVectorLayer )
    return QgsGeometry();

  switch ( mVectorLayer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
    {
      geom = new QgsPoint( mRubberbandModel->currentPoint( mVectorLayer->crs() ) );
      break;
    }
    case QgsWkbTypes::LineGeometry:
    {
      QgsLineString *line = new QgsLineString();
      line->setPoints( mRubberbandModel->pointSequence( mVectorLayer->crs() ) );
      geom = line;
      break;
    }
    case QgsWkbTypes::PolygonGeometry:
    {
      QgsPolygon *polygon = new QgsPolygon();
      QgsLineString *ring = new QgsLineString();
      ring->setPoints( mRubberbandModel->pointSequence( mVectorLayer->crs() ) );
      polygon->setExteriorRing( ring );
      geom = polygon;
      break;
    }

    break;
    case QgsWkbTypes::UnknownGeometry:
      break;
    case QgsWkbTypes::NullGeometry:
      break;
  }

  return QgsGeometry( geom );
}

QgsQuickRubberbandModel *QgsQuickGeometry::rubberbandModel() const
{
  return mRubberbandModel;
}

void QgsQuickGeometry::setRubberbandModel( QgsQuickRubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  mRubberbandModel = rubberbandModel;

  emit rubberbandModelChanged();
}

void QgsQuickGeometry::applyRubberband()
{
  // TODO: Will need to be implemented for multipart features or polygons with holes.
}

QgsVectorLayer *QgsQuickGeometry::vectorLayer() const
{
  return mVectorLayer;
}

void QgsQuickGeometry::setVectorLayer( QgsVectorLayer *vectorLayer )
{
  if ( mVectorLayer == vectorLayer )
    return;

  mVectorLayer = vectorLayer;
  emit vectorLayerChanged();
}
