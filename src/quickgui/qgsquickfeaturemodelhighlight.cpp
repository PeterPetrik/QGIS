/***************************************************************************
  qgsqguickfeaturemodelhighlight.cpp
  --------------------------------------
  Date                 : 9.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsvectorlayer.h"

#include "qgsquickfeaturemodel.h"
#include "qgsquickfeaturemodelhighlight.h"
#include "qgsquickmapsettings.h"
#include "qgsquicksgrubberband.h"


QgsQuickFeatureModelHighlight::QgsQuickFeatureModelHighlight( QQuickItem* parent )
  : QQuickItem( parent )
  , mModel( nullptr )
  , mDirty( false )
  , mMapSettings( nullptr )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );

  connect( this, &QgsQuickFeatureModelHighlight::modelChanged, this, &QgsQuickFeatureModelHighlight::onDataChanged );
}

void QgsQuickFeatureModelHighlight::onDataChanged()
{
  if ( mModel )
  {
    connect( mModel, &QgsQuickFeatureModel::modelReset, this, &QgsQuickFeatureModelHighlight::onModelDataChanged );
    connect( mModel, &QgsQuickFeatureModel::rowsRemoved, this, &QgsQuickFeatureModelHighlight::onModelDataChanged );
  }

  onModelDataChanged();
}

void QgsQuickFeatureModelHighlight::onModelDataChanged()
{
  mDirty = true;
  update();
}

QSGNode* QgsQuickFeatureModelHighlight::updatePaintNode( QSGNode* n, QQuickItem::UpdatePaintNodeData* )
{
  if ( !mDirty || !mMapSettings )
    return n;

  delete n;
  n = new QSGNode;

  if ( !mModel )
    return n;

  QgsVectorLayer* layer = mModel->layer();
  if ( layer )
  {
    QgsCoordinateTransform transf( layer->crs(), mMapSettings->destinationCrs() );

    QgsFeature feature = mModel->feature();
    QgsGeometry geom( feature.geometry() );
    geom.transform( transf );

    // TODO: this is very crude conversion! QgsQuickSGRubberband should accept any type of geometry
    QVector<QgsPoint> points;
    for ( auto it = geom.vertices_begin(); it != geom.vertices_end(); ++it )
      points.append( *it );

    QgsQuickSGRubberband* rb = new QgsQuickSGRubberband( points, geom.type(), mColor, mWidth );
    rb->setFlag( QSGNode::OwnedByParent );
    n->appendChildNode( rb );
  }
  mDirty = false;

  return n;
}
