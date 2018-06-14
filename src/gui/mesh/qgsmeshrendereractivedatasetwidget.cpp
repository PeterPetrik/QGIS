/***************************************************************************
    qgsmeshrendereractivedatasetwidget.cpp
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

#include "qgsmeshrendereractivedatasetwidget.h"

#include "qgis.h"
#include "qgsmapcanvas.h"
#include "qgsmeshlayer.h"
#include "qgsrasterlayer.h"
#include "raster/qgscolorrampshaderwidget.h"
#include "raster/qgsrasterminmaxwidget.h"
#include "qgsrasterminmaxorigin.h"
#include "qgsmessagelog.h"


QgsMeshRendererActiveDatasetWidget::QgsMeshRendererActiveDatasetWidget( QWidget *parent )
  : QWidget( parent )

{
  setupUi( this );
  connect( mDatasetGroupTree, &QgsMeshDatasetGroupTree::activeGroupChanged, this, &QgsMeshRendererActiveDatasetWidget::onActiveGroupChanged );
  connect( mDatasetSlider, &QSlider::valueChanged, this, &QgsMeshRendererActiveDatasetWidget::onActiveDatasetChanged );
  connect( mDisplayScalarsCheckBox, &QCheckBox::stateChanged, this, &QgsMeshRendererActiveDatasetWidget::onScalarChecked );
  connect( mDisplayVectorsCheckBox, &QCheckBox::stateChanged, this, &QgsMeshRendererActiveDatasetWidget::onVectorChecked );
  connect( mDisplayNativeMeshCheckBox, &QCheckBox::stateChanged, this, &QgsMeshRendererActiveDatasetWidget::onMeshChecked );
  connect( mDisplayTriangularMeshCheckBox, &QCheckBox::stateChanged, this, &QgsMeshRendererActiveDatasetWidget::onTringularMeshChecked );
}

QgsMeshRendererActiveDatasetWidget::~QgsMeshRendererActiveDatasetWidget() = default;

void QgsMeshRendererActiveDatasetWidget::setLayer( QgsMeshLayer *layer )
{
  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
  }

  setEnabled( mMeshLayer );
  syncToLayer();

  mDatasetGroupTree->setLayer( layer );
}

int QgsMeshRendererActiveDatasetWidget::activeScalarDataset() const
{
  if ( !isEnabled() ||
       !mDisplayScalarsCheckBox->isEnabled() ||
       !mDisplayScalarsCheckBox->isChecked() )
    return -1;
  return  datasetIndex();
}

int QgsMeshRendererActiveDatasetWidget::activeVectorDataset() const
{
  if ( !isEnabled() ||
       !mDisplayVectorsCheckBox->isEnabled() ||
       !mDisplayVectorsCheckBox->isChecked() )
    return -1;
  return  datasetIndex();
}

bool QgsMeshRendererActiveDatasetWidget::meshRenderingOn() const
{
  return isEnabled() && mDisplayNativeMeshCheckBox->isChecked();
}

bool QgsMeshRendererActiveDatasetWidget::triangularMeshRenderingOn() const
{
  return isEnabled() && mDisplayTriangularMeshCheckBox->isChecked();
}

void QgsMeshRendererActiveDatasetWidget::onActiveGroupChanged()
{
  const QVector<int> datasets = mDatasetGroupTree->datasetsInActiveGroup();

  mDatasetSlider->setMinimum( 0 );
  mDatasetSlider->setMaximum( datasets.size() - 1 );
  mDatasetSlider->setValue( 0 );

  qDebug() << "Size" << datasets.size() - 1;

}

void QgsMeshRendererActiveDatasetWidget::onActiveDatasetChanged( int value )
{
  int datasetIndex = -1;
  const QVector<int> datasets = mDatasetGroupTree->datasetsInActiveGroup();
  if ( datasets.size() < value || !mMeshLayer || !mMeshLayer->dataProvider() )
  {
    mDisplayScalarsCheckBox->setEnabled( false );
    mDisplayVectorsCheckBox->setEnabled( false );
  }
  else
  {
    datasetIndex = datasets[value];
    const QgsMeshDatasetMetadata meta = mMeshLayer->dataProvider()->datasetMetadata( datasetIndex );
    mDisplayScalarsCheckBox->setEnabled( true );
    mDisplayVectorsCheckBox->setEnabled( meta.isVector() );
  }

  updateMetadata( datasetIndex );

  emit activeScalarDatasetChanged( activeScalarDataset() );
  emit activeVectorDatasetChanged( activeVectorDataset() );

  emit widgetChanged();
}

void QgsMeshRendererActiveDatasetWidget::onScalarChecked( int toggle )
{
  Q_UNUSED( toggle );
  emit activeScalarDatasetChanged( activeScalarDataset() );
  emit widgetChanged();
}

void QgsMeshRendererActiveDatasetWidget::onVectorChecked( int toggle )
{
  Q_UNUSED( toggle );
  emit activeVectorDatasetChanged( activeVectorDataset() );
  emit widgetChanged();
}

void QgsMeshRendererActiveDatasetWidget::onMeshChecked( int toggle )
{
  Q_UNUSED( toggle );
  emit meshRenderingOnChanged( meshRenderingOn() );
  emit widgetChanged();
}

void QgsMeshRendererActiveDatasetWidget::onTringularMeshChecked( int toggle )
{
  Q_UNUSED( toggle );
  emit triangularMeshRenderingOnChange( triangularMeshRenderingOn() );
  emit widgetChanged();
}

void QgsMeshRendererActiveDatasetWidget::updateMetadata( int datasetIndex )
{
  if ( datasetIndex == -1 )
  {
    mActiveDatasetMetadata->setText( QString( "N/A" ) );
  }
  else
  {
    const QgsMeshDatasetMetadata meta = mMeshLayer->dataProvider()->datasetMetadata( datasetIndex );
    QString msg;
    msg += QStringLiteral( "<table>" );
    msg += QStringLiteral( "<tr><td>is on vertices</td><td>%1</td></tr>" ).arg( meta.isOnVertices() );
    msg += QStringLiteral( "<tr><td>is vector</td><td>%1</td></tr>" ).arg( meta.isVector() );
    for ( auto it = meta.extraOptions().constBegin(); it != meta.extraOptions().constEnd(); ++it )
    {
      msg += QStringLiteral( "<tr><td>%1</td><td>%2</td></tr>" ).arg( it.key() ).arg( it.value() );
    }
    msg += QStringLiteral( "</table>" );
    mActiveDatasetMetadata->setText( msg );
  }

}

int QgsMeshRendererActiveDatasetWidget::datasetIndex() const
{
  const QVector<int> datasets = mDatasetGroupTree->datasetsInActiveGroup();
  int value = mDatasetSlider->value();
  int datasetIndex = -1;
  if ( value < datasets.size() )
  {
    datasetIndex = datasets[value];
  }
  return datasetIndex;
}

void QgsMeshRendererActiveDatasetWidget::syncToLayer()
{
  if ( mMeshLayer )
  {
    whileBlocking( mDisplayNativeMeshCheckBox )->setChecked( mMeshLayer->rendererNativeMeshSettings().isEnabled() );
    whileBlocking( mDisplayTriangularMeshCheckBox )->setChecked( mMeshLayer->rendererTriangularMeshSettings().isEnabled() );
    whileBlocking( mDisplayScalarsCheckBox )->setChecked( mMeshLayer->activeScalarDataset() != -1 );
    whileBlocking( mDisplayVectorsCheckBox )->setChecked( mMeshLayer->activeVectorDataset() != -1 );
  }
}
