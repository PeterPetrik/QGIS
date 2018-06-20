/***************************************************************************
    qgsmeshrenderermeshsettingswidget.cpp
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

#include "qgsmeshrenderermeshsettingswidget.h"
#include <QtGlobal>

#include "qgis.h"
#include "qgsmapcanvas.h"
#include "qgsmeshlayer.h"
#include "qgsrasterlayer.h"
#include "raster/qgscolorrampshaderwidget.h"
#include "raster/qgsrasterminmaxwidget.h"
#include "qgsrasterminmaxorigin.h"
#include "qgsmessagelog.h"
#include "qgscolorbutton.h"
#include "qgsdoublespinbox.h"

QgsMeshRendererMeshSettingsWidget::QgsMeshRendererMeshSettingsWidget( QWidget *parent )
  : QWidget( parent )

{
  setupUi( this );

  connect( mColorWidget, &QgsColorButton::colorChanged, this, &QgsMeshRendererMeshSettingsWidget::widgetChanged );
  connect( mLineWidthSpinBox, QOverload<double>::of( &QgsDoubleSpinBox::valueChanged ),
           this, &QgsMeshRendererMeshSettingsWidget::widgetChanged );
}

void QgsMeshRendererMeshSettingsWidget::setLayer( QgsMeshLayer *layer, bool isTriangularMesh )
{
  mIsTriangularMesh = isTriangularMesh;

  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
    syncToLayer();
  }
}

QgsMeshRendererMeshSettings QgsMeshRendererMeshSettingsWidget::settings() const
{
  QgsMeshRendererMeshSettings settings;
  settings.setColor( mColorWidget->color() );
  settings.setLineWidth( mLineWidthSpinBox->value() );
  return settings;
}

void QgsMeshRendererMeshSettingsWidget::syncToLayer( )
{
  if ( !mMeshLayer )
    return;

  QgsMeshRendererMeshSettings settings;
  if ( mIsTriangularMesh )
    settings = mMeshLayer->rendererTriangularMeshSettings();
  else
    settings = mMeshLayer->rendererTriangularMeshSettings();

  mColorWidget->setColor( settings.color() );
  mLineWidthSpinBox->setValue( settings.lineWidth() );
}
