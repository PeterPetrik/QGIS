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

#include <QObject>
#include <QString>
#include <QUrl>

#include "qgis_quick.h"

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

  /**
    * Device pixels. Used to scale all pixel sizes for GUI elements.
    * Defaults to 1. Use QApplication::desktop()->physicalDpiX() to initialize.
    */
  Q_PROPERTY( qreal dp READ devicePixels WRITE setDevicePixels NOTIFY devicePixelsChanged )

  /**
    * Directory with the icons. If icon is not found in the theme directory, it is taken from
    * QgsQuick library resources. Use this to replace default images with the customized.
    *
    * /sa QgsQuickUtils::getThemeIcon()
    */
  Q_PROPERTY( QString theme READ themeDir WRITE setThemeDir NOTIFY themeDirChanged )

  /**
   * Use SVG instead of PNGs (needs Android API level 21 or above) for icons
   *
   * /sa QgsQuickUtils::getThemeIcon()
   */
  Q_PROPERTY( bool useVectorIcons READ useVectorIcons WRITE setUseVectorIcons NOTIFY useVectorIconsChanged )

public:
  explicit QgsQuickStyle(QObject *parent = 0);

  qreal devicePixels() const;
  QString themeDir() const;
  bool useVectorIcons() const;

  void setUseVectorIcons(bool use);
  void setDevicePixels(qreal dp);
  void setThemeDir(QString dir);

signals:
    void devicePixelsChanged();
    void themeDirChanged();
    void useVectorIconsChanged();

private:
    QString mThemeDir;
    qreal mDevicePixels;
    bool mUseVectorIcons;
};

#endif // QGSQUICKSTYLE_H
