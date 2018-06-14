/***************************************************************************
    qgsmeshrenderervectorsettingswidget.cpp
    ---------------------------------------
    begin                : June 2018
    copyright            : (C) 2018 by Peter Petrik
    email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsmeshrenderervectorsettingswidget.h"

#include "qgis.h"
#include "qgsmapcanvas.h"
#include "qgsmeshlayer.h"
#include "qgsrasterlayer.h"
#include "raster/qgscolorrampshaderwidget.h"
#include "raster/qgsrasterminmaxwidget.h"
#include "qgsrasterminmaxorigin.h"
#include "qgsmessagelog.h"


QgsMeshRendererVectorSettingsWidget::QgsMeshRendererVectorSettingsWidget( QWidget *parent )
  : QWidget( parent )

{
  setupUi( this );
}

void QgsMeshRendererVectorSettingsWidget::setLayer( QgsMeshLayer *layer )
{
  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
    syncToLayer();
  }
}

QgsMeshRendererVectorSettings QgsMeshRendererVectorSettingsWidget::settings() const
{
  QgsMeshRendererVectorSettings settings;
  return settings;
}

void QgsMeshRendererVectorSettingsWidget::setActiveDataset( int activeDataset )
{

}


QgsMeshRendererVectorSettingsWidget::~QgsMeshRendererVectorSettingsWidget() = default;

void QgsMeshRendererVectorSettingsWidget::syncToLayer( )
{
  if ( !mMeshLayer )
    return;
}

void QgsMeshRendererVectorSettingsWidget::refreshAfterStyleChanged()
{
}


