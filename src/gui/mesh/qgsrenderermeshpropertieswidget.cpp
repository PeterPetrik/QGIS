/***************************************************************************
    qgsrenderermeshpropertieswidget.cpp
    -----------------------------------
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

#include "qgsrenderermeshpropertieswidget.h"

#include "qgis.h"
#include "qgsmapcanvas.h"
#include "qgsmeshlayer.h"
#include "qgsrasterlayer.h"
#include "raster/qgscolorrampshaderwidget.h"
#include "raster/qgsrasterminmaxwidget.h"
#include "qgsrasterminmaxorigin.h"
#include "qgsmessagelog.h"
#include "qgsmeshrendererscalarsettingswidget.h"

QgsRendererMeshPropertiesWidget::QgsRendererMeshPropertiesWidget( QgsMapLayer *layer, QgsMapCanvas *canvas, QWidget *parent )
  : QgsMapLayerConfigWidget( layer, canvas, parent )

{
  mMeshLayer = qobject_cast<QgsMeshLayer *>( layer );
  if ( !mMeshLayer )
    return;

  if ( !mMapCanvas )
    return;

  setupUi( this );
  mMeshRendererScalarSettingsWidget->setLayer( mMeshLayer );
  connect( mMeshRendererScalarSettingsWidget, &QgsMeshRendererScalarSettingsWidget::widgetChanged, this, &QgsPanelWidget::widgetChanged );
}

QgsRendererMeshPropertiesWidget::~QgsRendererMeshPropertiesWidget() = default;

void QgsRendererMeshPropertiesWidget::apply()
{
  if ( !mMeshLayer )
    return;

  QgsMeshRendererScalarSettings settings = mMeshRendererScalarSettingsWidget->settings();
  mMeshLayer->setActiveScalarDataset( mMeshRendererScalarSettingsWidget->currentDataset() ); // bands are numbered from 1, datasets from 0
  mMeshLayer->setRendererScalarSettings( settings );
}
