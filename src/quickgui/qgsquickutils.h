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
#include <QUrl>

class QgsQuickPictureSource;

class QUICK_EXPORT QgsQuickUtils: public QObject
{
  Q_OBJECT

  Q_PROPERTY( qreal dp MEMBER mDevicePixels NOTIFY devicePixelsChanged )
  Q_PROPERTY( QString theme MEMBER mThemeDir NOTIFY themeDirChanged )

  public:
    static QgsQuickUtils* instance();

    // Common
    Q_INVOKABLE bool fileExists(QString path);

    // Themes
    Q_INVOKABLE QUrl getThemeIcon(const QString& name); //from custom theme dir or default if not found in the theme dir

    // Android picture capture
    Q_INVOKABLE QgsQuickPictureSource* getPicture( const QString &prefix );
    Q_INVOKABLE void open( const QString& data, const QString& type );


signals:
    void devicePixelsChanged();
    void themeDirChanged();

  protected:
    explicit QgsQuickUtils(QObject *parent = 0);
    static QgsQuickUtils* sInstance;

    // Themes
    QString mThemeDir;

    /* Visual settings */
    qreal mDevicePixels;
};

#endif // QGSQUICKUTILS_H
