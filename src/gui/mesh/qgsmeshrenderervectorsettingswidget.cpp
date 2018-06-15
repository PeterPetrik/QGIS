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

  connect( mColorWidget, &QgsColorButton::colorChanged, this, &QgsMeshRendererVectorSettingsWidget::widgetChanged );
  connect( mLineWidthSpinBox, QOverload<double>::of( &QgsDoubleSpinBox::valueChanged ),
           this, &QgsMeshRendererVectorSettingsWidget::widgetChanged );

  connect( mShaftLengthComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ),
           this, &QgsMeshRendererVectorSettingsWidget::widgetChanged );

  connect( mShaftLengthComboBox, QOverload<int>::of( &QComboBox::currentIndexChanged ),
           mShaftOptionsStackedWidget, &QStackedWidget::setCurrentIndex );

  QVector<QLineEdit *> widgets;
  widgets << mMinMagLineEdit << mMinMagLineEdit
          << mHeadWidthLineEdit << mHeadLengthLineEdit
          << mMinimumShaftLineEdit << mMaximumShaftLineEdit
          << mScaleShaftByFactorOfLineEdit << mShaftLengthLineEdit;

  for ( auto widget : widgets )
  {
    connect( widget, &QLineEdit::textChanged, this, &QgsMeshRendererVectorSettingsWidget::widgetChanged );
    connect( widget, &QLineEdit::textEdited, this, &QgsMeshRendererVectorSettingsWidget::widgetChanged );
  }
}

QgsMeshRendererVectorSettingsWidget::~QgsMeshRendererVectorSettingsWidget() = default;

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

  // basic
  settings.setColor( mColorWidget->color() );
  settings.setLineWidth( mLineWidthSpinBox->value() );

  // filter by magnitude
  double val = filterValue( mMinMagLineEdit->text(), -1 );
  settings.setFilterMin( val );

  val = filterValue( mMinMagLineEdit->text(), -1 );
  settings.setFilterMin( val );

  // arrow head
  val = filterValue( mHeadWidthLineEdit->text(), settings.arrowHeadWidthRatio() );
  settings.setArrowHeadWidthRatio( val );

  val = filterValue( mHeadLengthLineEdit->text(), settings.arrowHeadLengthRatio() );
  settings.setArrowHeadLengthRatio( val );

  // shaft length
  auto method = static_cast<QgsMeshRendererVectorSettings::ArrowScalingMethod>( mShaftLengthComboBox->currentIndex() );
  settings.setShaftLengthMethod( method );

  val = filterValue( mMinimumShaftLineEdit->text(), settings.minShaftLength() );
  settings.setMinShaftLength( val );

  val = filterValue( mMaximumShaftLineEdit->text(), settings.maxShaftLength() );
  settings.setMaxShaftLength( val );

  val = filterValue( mScaleShaftByFactorOfLineEdit->text(), settings.scaleFactor() );
  settings.setScaleFactor( val );

  val = filterValue( mShaftLengthLineEdit->text(), settings.fixedShaftLength() );
  settings.setFixedShaftLength( val );

  return settings;
}

void QgsMeshRendererVectorSettingsWidget::setActiveDataset( int activeDataset )
{
  mActiveDataset = activeDataset;
}

void QgsMeshRendererVectorSettingsWidget::syncToLayer( )
{
  if ( !mMeshLayer )
    return;

  QgsMeshRendererVectorSettings settings = mMeshLayer->rendererVectorSettings();

  // basic
  mColorWidget->setColor( settings.color() );
  mLineWidthSpinBox->setValue( settings.lineWidth() );

  // filter by magnitude
  if ( settings.filterMin() > 0 )
  {
    mMinMagLineEdit->setText( QString::number( settings.filterMin() ) );
  }
  if ( settings.filterMax() > 0 )
  {
    mMaxMagLineEdit->setText( QString::number( settings.filterMax() ) );
  }

  // arrow head
  mHeadWidthLineEdit->setText( QString::number( settings.arrowHeadWidthRatio() ) );
  mHeadLengthLineEdit->setText( QString::number( settings.arrowHeadLengthRatio() ) );

  // shaft length
  mShaftLengthComboBox->setCurrentIndex( settings.shaftLengthMethod() );

  mMinimumShaftLineEdit->setText( QString::number( settings.minShaftLength() ) );
  mMaximumShaftLineEdit->setText( QString::number( settings.maxShaftLength() ) );
  mScaleShaftByFactorOfLineEdit->setText( QString::number( settings.scaleFactor() ) );
  mShaftLengthLineEdit->setText( QString::number( settings.fixedShaftLength() ) );

}

double QgsMeshRendererVectorSettingsWidget::filterValue( const QString &text, double err_val ) const
{
  if ( text.isEmpty() )
    return err_val;

  bool ok;
  double val = text.toDouble( &ok );
  if ( !ok )
    return err_val;

  if ( val < 0 )
    return err_val;

  return val;
}

void QgsMeshRendererVectorSettingsWidget::refreshAfterStyleChanged()
{
}


