/***************************************************************************
 qgsquicksrubberband.cpp
  --------------------------------------
  Date                 : 11.6.2016
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

#include "qgsquickrubberband.h"
#include "qgsquickrubberbandmodel.h"
#include "qgsquicksgrubberband.h"

QgsQuickRubberband::QgsQuickRubberband( QQuickItem *parent )
  : QQuickItem( parent )
  , mModel( nullptr )
  , mDirty( false )
  , mColor( 192, 57, 43, 200 )
  , mWidth( 1.5 )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );
}

QgsQuickRubberbandModel *QgsQuickRubberband::model() const
{
  return mModel;
}

void QgsQuickRubberband::setModel( QgsQuickRubberbandModel *model )
{
  if ( mModel == model )
    return;

  if ( mModel )
  {
    disconnect( mModel, &QgsQuickRubberbandModel::vertexChanged, this, &QgsQuickRubberband::markDirty );
    disconnect( mModel, &QgsQuickRubberbandModel::verticesRemoved, this, &QgsQuickRubberband::markDirty );
    disconnect( mModel, &QgsQuickRubberbandModel::verticesInserted, this, &QgsQuickRubberband::markDirty );
  }

  mModel = model;

  if ( mModel )
  {
    connect( mModel, &QgsQuickRubberbandModel::vertexChanged, this, &QgsQuickRubberband::markDirty );
    connect( mModel, &QgsQuickRubberbandModel::verticesRemoved, this, &QgsQuickRubberband::markDirty );
    connect( mModel, &QgsQuickRubberbandModel::verticesInserted, this, &QgsQuickRubberband::markDirty );
  }

  markDirty();
  emit modelChanged();
}

QgsQuickMapSettings *QgsQuickRubberband::mapSettings() const
{
  return mMapSettings;
}

void QgsQuickRubberband::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;
  markDirty();

  emit mapSettingsChanged();
}

void QgsQuickRubberband::markDirty()
{
  mDirty = true;
  update();
}

QSGNode *QgsQuickRubberband::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty && mModel )
  {
    delete n;
    n = new QSGNode;

    if ( !mModel->isEmpty() )
    {
      QgsQuickSGRubberband *rb = new QgsQuickSGRubberband( mModel->flatVertices(), mModel->geometryType(), mColor, mWidth );
      rb->setFlag( QSGNode::OwnedByParent );

      n->appendChildNode( rb );
    }
  }

  return n;
}

qreal QgsQuickRubberband::width() const
{
  return mWidth;
}

void QgsQuickRubberband::setWidth( qreal width )
{
  mWidth = width;
}

QColor QgsQuickRubberband::color() const
{
  return mColor;
}

void QgsQuickRubberband::setColor( const QColor &color )
{
  mColor = color;
}
