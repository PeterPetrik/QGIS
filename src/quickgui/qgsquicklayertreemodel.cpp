/***************************************************************************
  qgsquicklayertreemodel.cpp
  --------------------------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquicklayertreemodel.h"

#include <qgslayertreemodel.h>
#include <qgslayertreenode.h>
#include <qgslayertree.h>
#include <qgsvectorlayer.h>
#include <qgslayertreemodellegendnode.h>

#include <QString>

QgsQuickLayerTreeModel::QgsQuickLayerTreeModel( QgsLayerTree* layerTree, QObject* parent )
  : QSortFilterProxyModel( parent )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  setSourceModel( mLayerTreeModel );
}

QVariant QgsQuickLayerTreeModel::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case Name:
    {
      QgsLayerTreeNode* node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
        return QVariant( nodeLayer->layer()->name() );
      }
      else
      {
        return QVariant();
      }
    }
    default:
      return QSortFilterProxyModel::data( index, role );
  }
}

QHash<int, QByteArray> QgsQuickLayerTreeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QSortFilterProxyModel::roleNames();
  roleNames[Name] = "name";
  return roleNames;
}
