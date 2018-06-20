/***************************************************************************
    qgsmeshdatasetgrouptreeview.cpp
    -------------------------------
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

#include "qgsmeshdatasetgrouptreeview.h"

#include "qgis.h"
#include "qgsmeshlayer.h"

#include <QList>
#include <QItemSelectionModel>

static QList<QStandardItem *> prepareRow(const QString &str)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(str);
    return rowItems;
}

QgsMeshDatasetGroupTreeView::QgsMeshDatasetGroupTreeView( QWidget *parent )
  : QTreeView( parent )
{
  setSelectionMode(QAbstractItemView::SingleSelection) ;
  connect( selectionModel(),
           &QItemSelectionModel::selectionChanged,
           this,
           &QgsMeshDatasetGroupTreeView::onSelectionChanged
           );
}

void QgsMeshDatasetGroupTreeView::setLayer( QgsMeshLayer *layer )
{
  if ( layer != mMeshLayer )
  {
    mMeshLayer = layer;
    repopulateTree();
  }
}

QVector<int> QgsMeshDatasetGroupTreeView::datasetsInActiveGroup() const
{
  if ( mGroups.constFind( mActiveGroup ) == mGroups.constEnd() )
    return QVector<int>();
  else
    return mGroups[mActiveGroup];
}

void QgsMeshDatasetGroupTreeView::onSelectionChanged( const QItemSelection &selected, const QItemSelection &deselected )
{
  Q_UNUSED( deselected );

  if ( selected.isEmpty() ) {
    mActiveGroup = QString();
    return;
  }

  if ( selected.first().indexes().isEmpty() ) {
    mActiveGroup = QString();
    return;
  }

  QModelIndex index = selected.first().indexes().first(); //single selection only
  QStandardItem* item = mModel.itemFromIndex(index);
  QString name = item->text();
  mActiveGroup = name;

  emit activeGroupChanged();
}

void QgsMeshDatasetGroupTreeView::repopulateTree()
{
  mGroups.clear();
  mActiveGroup.clear();
  mModel.clear();

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

  for ( int i = 0; i < groupsSorted.size(); ++i )
  {
    QString groupName = groupsSorted[i];
    QVector<int> datasets = mGroups[groupName];
    qSort( datasets );
    mGroups[groupName] = datasets;
    QString name = QString( "%1" ).arg( groupName );
    QStandardItem *root = mModel.invisibleRootItem();
    root->appendRow(prepareRow(name));
  }

  if (groupsSorted.size() > 0)
    setCurrentIndex(mModel.index(0, 0));
}

