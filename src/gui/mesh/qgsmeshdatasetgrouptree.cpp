/***************************************************************************
    qgsmeshdatasetgrouptree.cpp
    ---------------------------
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

#include "qgsmeshdatasetgrouptree.h"

#include "qgis.h"
#include "qgsmeshlayer.h"

QgsMeshDatasetGroupTree::QgsMeshDatasetGroupTree( QWidget *parent )
  : QTreeWidget( parent )

{
  setHeaderLabel( tr( "Datasets" ) );
  setColumnCount( 1 );
  connect( this, &QTreeWidget::currentItemChanged, this, &QgsMeshDatasetGroupTree::onSelectionChanged );
}

void QgsMeshDatasetGroupTree::setLayer( QgsMeshLayer *layer )
{
  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
    repopulateTree();
  }
}

QVector<int> QgsMeshDatasetGroupTree::datasetsInActiveGroup() const
{
  if ( mGroups.constFind( mActiveGroup ) == mGroups.constEnd() )
    return QVector<int>();
  else
    return mGroups[mActiveGroup];
}

void QgsMeshDatasetGroupTree::onSelectionChanged( QTreeWidgetItem *current, QTreeWidgetItem *previous )
{
  Q_UNUSED( previous );

  if ( current )
  {
    mActiveGroup = current->text( 0 );
  }
  else
  {
    mActiveGroup = QString();
  }

  emit activeGroupChanged();
}

void QgsMeshDatasetGroupTree::repopulateTree()
{
  mGroups.clear();
  mActiveGroup.clear();
  clear();

  if ( !mMeshLayer || !mMeshLayer->dataProvider() )
    return;

  for ( int i = 0; i < mMeshLayer->dataProvider()->datasetCount(); ++i )
  {
    // TODO name to metadata directly when groups are introduced in MDAL
    const QgsMeshDatasetMetadata meta = mMeshLayer->dataProvider()->datasetMetadata( i );
    QString name = meta.extraOptions()["name"];
    if ( mGroups.constFind( name ) == mGroups.constEnd() )
    {
      QVector<int> datasets;
      datasets.append( i );
      mGroups[name] = datasets;
    }
    else
    {
      mGroups[name].append( i );
    }
  }

  QStringList groupsSorted = mGroups.keys();
  qSort( groupsSorted.begin(), groupsSorted.end() );

  QList<QTreeWidgetItem *> items;
  for ( int i = 0; i < groupsSorted.size(); ++i )
  {
    QString groupName = groupsSorted[i];
    QVector<int> datasets = mGroups[groupName];
    qSort( datasets );
    mGroups[groupName] = datasets;
    QTreeWidgetItem *item = new QTreeWidgetItem( QStringList( QString( "%1" ).arg( groupName ) ) ) ;
    items.append( item );
  }
  insertTopLevelItems( 0, items );

  if ( topLevelItem( 0 ) )
    setCurrentItem( topLevelItem( 0 ), 0, QItemSelectionModel::Select );
}

