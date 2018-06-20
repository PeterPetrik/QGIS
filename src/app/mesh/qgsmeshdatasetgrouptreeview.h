/***************************************************************************
    qgsmeshdatasetgrouptreeview.h
    -----------------------------
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

#ifndef QGSMESHDATASETGROUPTREE_H
#define QGSMESHDATASETGROUPTREE_H

#include "qgis_app.h"

#include <QObject>
#include <QTreeView>
#include <QMap>
#include <QVector>
#include <QItemSelection>
#include <QStandardItemModel>

class QgsMeshLayer;

/**
 * Tree widget for display of the mesh dataset groups.
 * Dataset group is set of datasets with the same name,
 * but different control variable (e.g. time)
 *
 * One dataset group is selected (active)
 */
class APP_EXPORT QgsMeshDatasetGroupTreeView : public QTreeView
{
    Q_OBJECT

  public:
    QgsMeshDatasetGroupTreeView( QWidget *parent = nullptr );
    ~QgsMeshDatasetGroupTreeView() = default;

    //! Associates mesh layer with the widget
    void setLayer( QgsMeshLayer *layer );

    //! Returns all the dataset indexes in the active group
    QVector<int> datasetsInActiveGroup() const;

  signals:
    //! Selected dataset group changed
    void activeGroupChanged();

  private slots:
    void onSelectionChanged( const QItemSelection &selected, const QItemSelection &deselected );

  private:
    void repopulateTree();

    QStandardItemModel mModel;
    QgsMeshLayer *mMeshLayer = nullptr; // not owned
    QMap<QString, QVector<int>> mGroups; // group name -> dataset indices
    QString mActiveGroup;
};

#endif // QGSMESHRENDERERSCALARSETTINGSWIDGET_H
