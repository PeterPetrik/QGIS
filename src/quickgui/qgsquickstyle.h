/***************************************************************************
  qgsquickstyle.h
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

#ifndef QGSQUICKSTYLE_H
#define QGSQUICKSTYLE_H

#include "qgis_quick.h"

#include <QObject>
#include <QString>
#include <QUrl>

/**
 * \ingroup quick
 *
 * Singleton encapsulating the visual (color, icons, ...) properties for Items in the QgsQuick library.
 *
 * \note QML Type: Style (singleton)
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickStyle: public QObject
{
  Q_OBJECT

  Q_PROPERTY( qreal dp READ devicePixels WRITE setDevicePixels NOTIFY devicePixelsChanged )
  Q_PROPERTY( QString theme READ themeDir WRITE setThemeDir NOTIFY themeDirChanged )

public:
  explicit QgsQuickStyle(QObject *parent = 0);

  qreal devicePixels() const;
  QString themeDir() const;

  void setDevicePixels(qreal dp);
  void setThemeDir(QString dir);

signals:
    void devicePixelsChanged();
    void themeDirChanged();

private:
    QString mThemeDir;
    qreal mDevicePixels;
};

#endif // QGSQUICKSTYLE_H
