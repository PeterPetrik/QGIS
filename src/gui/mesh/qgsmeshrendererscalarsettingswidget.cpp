/***************************************************************************
    qgsmeshrendererscalarsettingswidget.cpp
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

#include "qgsmeshrendererscalarsettingswidget.h"

#include "qgis.h"
#include "qgsmeshlayer.h"
#include "qgsmessagelog.h"


QgsMeshRendererScalarSettingsWidget::QgsMeshRendererScalarSettingsWidget( QWidget *parent )
  : QWidget( parent )

{
  setupUi( this );

  connect( mScalarRecalculateMinMaxButton, &QPushButton::clicked, this, &QgsMeshRendererScalarSettingsWidget::recalculateMinMaxButtonClicked );
  connect( mScalarMinLineEdit, &QLineEdit::textChanged, this, &QgsMeshRendererScalarSettingsWidget::minMaxChanged );
  connect( mScalarMaxLineEdit, &QLineEdit::textChanged, this, &QgsMeshRendererScalarSettingsWidget::minMaxChanged );
  connect( mScalarMinLineEdit, &QLineEdit::textEdited, this, &QgsMeshRendererScalarSettingsWidget::minMaxEdited );
  connect( mScalarMaxLineEdit, &QLineEdit::textEdited, this, &QgsMeshRendererScalarSettingsWidget::minMaxEdited );
  connect( mScalarColorRampShaderWidget, &QgsColorRampShaderWidget::widgetChanged, this, &QgsMeshRendererScalarSettingsWidget::widgetChanged );

}

void QgsMeshRendererScalarSettingsWidget::setLayer( QgsMeshLayer *layer )
{
  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
    syncToLayer();
  }
}

QgsMeshRendererScalarSettings QgsMeshRendererScalarSettingsWidget::settings() const
{
  QgsMeshRendererScalarSettings settings;
  settings.setColorRampShader( mScalarColorRampShaderWidget->shader() );
  return settings;
}

void QgsMeshRendererScalarSettingsWidget::syncToLayer( )
{
  if ( !mMeshLayer )
    return;

  if ( mMeshLayer->rendererScalarSettings().colorRampShader() )
  {
    whileBlocking( mScalarMinLineEdit )->setText( QString::number( mMeshLayer->rendererScalarSettings().colorRampShader()->minimumValue() ) );
    whileBlocking( mScalarMaxLineEdit )->setText( QString::number( mMeshLayer->rendererScalarSettings().colorRampShader()->maximumValue() ) );
    whileBlocking( mScalarColorRampShaderWidget )->setFromShader( mMeshLayer->rendererScalarSettings().colorRampShader() );
  }
}

double QgsMeshRendererScalarSettingsWidget::lineEditValue( const QLineEdit *lineEdit ) const
{
  if ( lineEdit->text().isEmpty() )
  {
    return std::numeric_limits<double>::quiet_NaN();
  }

  return lineEdit->text().toDouble();
}

void QgsMeshRendererScalarSettingsWidget::minMaxChanged()
{
  double min = lineEditValue( mScalarMinLineEdit );
  double max = lineEditValue( mScalarMaxLineEdit );
  mScalarColorRampShaderWidget->setMinMax( min, max );
}

void QgsMeshRendererScalarSettingsWidget::minMaxEdited()
{
  double min = lineEditValue( mScalarMinLineEdit );
  double max = lineEditValue( mScalarMaxLineEdit );
  mScalarColorRampShaderWidget->setMinMaxAndClassify( min, max );
}

void QgsMeshRendererScalarSettingsWidget::recalculateMinMaxButtonClicked()
{
  double min, max;
  calcMinMax( mActiveDataset, min, max );
  whileBlocking( mScalarMinLineEdit )->setText( QString::number( min ) );
  whileBlocking( mScalarMaxLineEdit )->setText( QString::number( max ) );
  mScalarColorRampShaderWidget->setMinMaxAndClassify( min, max );
}

void QgsMeshRendererScalarSettingsWidget::setActiveDataset( int activeDataset )
{
  mActiveDataset = activeDataset;
}

void QgsMeshRendererScalarSettingsWidget::calcMinMax( int datasetIndex, double &min, double &max ) const
{
  if ( !mMeshLayer )
    return;

  if ( !mMeshLayer->dataProvider() )
    return;

  const QgsMeshDatasetMetadata metadata = mMeshLayer->dataProvider()->datasetMetadata( datasetIndex );
  bool scalarDataOnVertices = metadata.isOnVertices();
  int count;
  if ( scalarDataOnVertices )
    count = mMeshLayer->dataProvider()->vertexCount();
  else
    count = mMeshLayer->dataProvider()->faceCount();

  bool myFirstIterationFlag = true;
  for ( int i = 0; i < count; ++i )
  {
    double myValue = mMeshLayer->dataProvider()->datasetValue( datasetIndex, i ).scalar();
    if ( std::isnan( myValue ) ) continue; // NULL
    if ( myFirstIterationFlag )
    {
      myFirstIterationFlag = false;
      min = myValue;
      max = myValue;
    }
    else
    {
      if ( myValue < min )
      {
        min = myValue;
      }
      if ( myValue > max )
      {
        max = myValue;
      }
    }
  }
}
