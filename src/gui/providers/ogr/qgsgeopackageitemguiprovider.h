/***************************************************************************
      qgsgeopackageitemguiprovider.h
      -------------------
    begin                : June, 2019
    copyright            : (C) 2019 by Peter Petrik
    email                : zilolv at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSGEOPACKAGEITEMGUIPROVIDER_H
#define QGSGEOPACKAGEITEMGUIPROVIDER_H

#include <QObject>
#include "qgsdataitemguiprovider.h"

class QgsGeoPackageCollectionItem;

class QgsGeoPackageItemGuiProvider : public QObject, public QgsDataItemGuiProvider
{
    Q_OBJECT

  public:

    QgsGeoPackageItemGuiProvider() = default;

    QString name() override { return QStringLiteral( "geopackage_items" ); }

    void populateContextMenu( QgsDataItem *item, QMenu *menu,
                              const QList<QgsDataItem *> &selectedItems, QgsDataItemGuiContext context ) override;

    bool acceptDrop( QgsDataItem *item, QgsDataItemGuiContext context ) override;
    bool handleDrop( QgsDataItem *item, QgsDataItemGuiContext context, const QMimeData *data, Qt::DropAction action ) override;

  private:
    static bool handleDropGeopackage( QgsGeoPackageCollectionItem* item, const QMimeData *data);
};

#endif // QGSGEOPACKAGEITEMGUIPROVIDER_H
