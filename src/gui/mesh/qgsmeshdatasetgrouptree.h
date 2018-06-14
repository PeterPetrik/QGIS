/***************************************************************************
    qgsmeshdatasetgrouptree.h
    -------------------------
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

#include <QObject>
#include <QTreeWidget>
#include "qgis_gui.h"
#include <QMap>
#include <QVector>

class QgsMeshLayer;

/**
 * \ingroup gui
 * \class QgsMeshDatasetGroupTree
 */
class GUI_EXPORT QgsMeshDatasetGroupTree : public QTreeWidget
{
    Q_OBJECT

  public:

    /**
     * A widget to hold the renderer scalar settings for a mesh layer.
     * \param parent Parent object
     */
    QgsMeshDatasetGroupTree( QWidget *parent = nullptr );
    ~QgsMeshDatasetGroupTree();

    void setLayer( QgsMeshLayer *layer );

    QVector<int> datasetsInActiveGroup() const;

  signals:
    //! Datasets in active/selected group changed
    void activeGroupChanged();

  private slots:
    void onSelectionChanged( QTreeWidgetItem *current, QTreeWidgetItem *previous );

  private:
    void repopulateTree();

    QgsMeshLayer *mMeshLayer = nullptr;
    QMap<QString, QVector<int>> mGroups; // value-> dataset numbers
    QString mActiveGroup = QString();
};

#endif // QGSMESHRENDERERSCALARSETTINGSWIDGET_H
