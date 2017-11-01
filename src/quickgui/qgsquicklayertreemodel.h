/***************************************************************************
  qgsquicklayertreemodel.h
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


#ifndef QGSQUICKLAYERTREEMODEL_H
#define QGSQUICKLAYERTREEMODEL_H

#include <QSortFilterProxyModel>

class QgsLayerTree;
class QgsLayerTreeModel;
class QgsProject;

class QgsQuickLayerTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

  public:
    enum Roles
    {
      Name = Qt::UserRole + 1
    };
    Q_ENUMS( Roles )

    explicit QgsQuickLayerTreeModel( QgsLayerTree* layerTree, QObject* parent = nullptr );

    Q_INVOKABLE QVariant data( const QModelIndex& index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

  signals:
    void mapThemeChanged();

  private:
    QgsLayerTreeModel* mLayerTreeModel;
    QString mMapTheme;
    QgsProject* mProject;
};

#endif // QGSQUICKLAYERTREEMODEL_H
