/***************************************************************************
  qgsquickutils.h
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

#ifndef QGSQUICKUTILS_H
#define QGSQUICKUTILS_H

#include "qgis_quick.h"

#include <QObject>
#include <QString>

class PictureSource;

class QUICK_EXPORT QgsQuickUtils: public QObject
{
  Q_OBJECT

  Q_PROPERTY( qreal dp MEMBER mDevicePixels WRITE setDevicePixels NOTIFY devicePixelsChanged )

  public:
    static QgsQuickUtils* instance();


    virtual PictureSource* getPicture( const QString &prefix );
    virtual void open( const QString& data, const QString& type );

    void setDevicePixels(qreal dp);

signals:
    void devicePixelsChanged();

  protected:
    explicit QgsQuickUtils(QObject *parent = 0);
    static QgsQuickUtils* sInstance;

    /* Visual settings */
    qreal mDevicePixels;
};

#endif // QGSQUICKUTILS_H
