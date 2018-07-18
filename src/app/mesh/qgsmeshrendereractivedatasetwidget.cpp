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
#include "qgsmeshlayer.h"
#include "qgsmessagelog.h"
#include "qgsmeshrenderersettings.h"

QgsMeshRendererActiveDatasetWidget::QgsMeshRendererActiveDatasetWidget( QWidget *parent )
  : QWidget( parent )
{
  setupUi( this );
  connect( mDatasetGroupTreeView, &QgsMeshDatasetGroupTreeView::activeGroupChanged, this, &QgsMeshRendererActiveDatasetWidget::onActiveGroupChanged );
  connect( mDatasetSlider, &QSlider::valueChanged, this, &QgsMeshRendererActiveDatasetWidget::onActiveDatasetChanged );
}

void QgsMeshRendererActiveDatasetWidget::setLayer( QgsMeshLayer *layer )
{
  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
  }

  mDatasetGroupTreeView->setLayer( layer );
  setEnabled( mMeshLayer );
  syncToLayer();
}

QgsMeshDatasetIndex QgsMeshRendererActiveDatasetWidget::activeScalarDataset() const
{
  return mActiveScalarDataset;
}

QgsMeshDatasetIndex QgsMeshRendererActiveDatasetWidget::activeVectorDataset() const
{
  return mActiveVectorDataset;
}

void QgsMeshRendererActiveDatasetWidget::onActiveGroupChanged()
{
  int datasetCount = mMeshLayer->dataProvider()->datasetCount( mDatasetGroupTreeView->activeGroup() );

  mDatasetSlider->setMinimum( 0 );
  mDatasetSlider->setMaximum( datasetCount - 1 );
  mDatasetSlider->setValue( 0 );
}

void QgsMeshRendererActiveDatasetWidget::onActiveDatasetChanged( int value )
{
  int groupIndex = mDatasetGroupTreeView->activeGroup();
  int datasetCount = mMeshLayer->dataProvider()->datasetCount( groupIndex );

  mActiveScalarDataset = QgsMeshDatasetIndex();
  mActiveVectorDataset = QgsMeshDatasetIndex();
  QgsMeshDatasetIndex datasetIndex( groupIndex, value );

  if ( datasetCount > value && mMeshLayer && mMeshLayer->dataProvider() )
  {
    const QgsMeshDatasetGroupMetadata meta = mMeshLayer->dataProvider()->datasetGroupMetadata( datasetIndex );
    mActiveScalarDataset = datasetIndex;
    if ( meta.isVector() )
      mActiveVectorDataset = datasetIndex;
  }

  updateMetadata( datasetIndex );

  emit activeScalarDatasetChanged( activeScalarDataset() );
  emit activeVectorDatasetChanged( activeVectorDataset() );

  emit widgetChanged();
}

void QgsMeshRendererActiveDatasetWidget::updateMetadata( QgsMeshDatasetIndex datasetIndex )
{
  if ( !datasetIndex.isValid() )
  {
    mActiveDatasetMetadata->setText( tr( "No dataset selected" ) );
  }
  else
  {
    QString msg;
    msg += QStringLiteral( "<table>" );

    const QgsMeshDatasetMetadata meta = mMeshLayer->dataProvider()->datasetMetadata( datasetIndex );
    msg += QStringLiteral( "<tr><td>%1</td><td>%2</td></tr>" )
           .arg( tr( "Is valid" ) )
           .arg( meta.isValid() ? tr( "Yes" ) : tr( "No" ) );

    msg += QStringLiteral( "<tr><td>%1</td><td>%2</td></tr>" )
           .arg( tr( "Time" ) )
           .arg( meta.time() );

    const QgsMeshDatasetGroupMetadata gmeta = mMeshLayer->dataProvider()->datasetGroupMetadata( datasetIndex );
    msg += QStringLiteral( "<tr><td>%1</td><td>%2</td></tr>" )
           .arg( tr( "Is on vertices" ) )
           .arg( gmeta.isOnVertices() ? tr( "Yes" ) : tr( "No" ) );

    msg += QStringLiteral( "<tr><td>%1</td><td>%2</td></tr>" )
           .arg( tr( "Is vector" ) )
           .arg( gmeta.isVector() ? tr( "Yes" ) : tr( "No" ) );

    for ( auto it = gmeta.extraOptions().constBegin(); it != gmeta.extraOptions().constEnd(); ++it )
    {
      msg += QStringLiteral( "<tr><td>%1</td><td>%2</td></tr>" ).arg( it.key() ).arg( it.value() );
    }

    msg += QStringLiteral( "</table>" );
    mActiveDatasetMetadata->setText( msg );
  }

}

QgsMeshDatasetIndex QgsMeshRendererActiveDatasetWidget::datasetIndex() const
{
  int value = mDatasetSlider->value();
  int groupIndex = mDatasetGroupTreeView->activeGroup();

  if ( mMeshLayer &&
       mMeshLayer->dataProvider() &&
       mMeshLayer->dataProvider()->datasetCount( groupIndex ) > value
     )
    return QgsMeshDatasetIndex( groupIndex, value );
  else
    return QgsMeshDatasetIndex();
}

void QgsMeshRendererActiveDatasetWidget::syncToLayer()
{
  mDatasetGroupTreeView->syncToLayer();

  if ( mMeshLayer )
  {
    mActiveScalarDataset = mMeshLayer->activeScalarDataset();
    mActiveVectorDataset = mMeshLayer->activeVectorDataset();
  }
  else
  {
    mActiveScalarDataset = QgsMeshDatasetIndex();
    mActiveVectorDataset = QgsMeshDatasetIndex();
  }

  if ( mActiveScalarDataset.isValid() )
    whileBlocking( mDatasetSlider )->setValue( mActiveScalarDataset.dataset() );
}
