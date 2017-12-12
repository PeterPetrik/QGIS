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

#include "qgspointxy.h"
#include "qgspoint.h"

#include <QObject>
#include <QString>
#include <QUrl>

class QgsQuickPictureSource;
class QgsQuickStyle;
class QgsCoordinateReferenceSystem;

class QUICK_EXPORT QgsQuickUtils: public QObject
{
  Q_OBJECT

  public:
    static QgsQuickUtils* instance();

    // CRS and geometry
    Q_INVOKABLE QgsCoordinateReferenceSystem coordinateReferenceSystemFromEpsgId(long epsg) const; //if you want to create CRS from QML
    Q_INVOKABLE QgsPointXY pointXYFactory(double x, double y) const;
    Q_INVOKABLE QgsPoint pointFactory(double x, double y) const;
    Q_INVOKABLE QgsPointXY transformPoint(QgsCoordinateReferenceSystem srcCrs, QgsCoordinateReferenceSystem destCrs, QgsPointXY srcPoint) const;

    // Common
    Q_INVOKABLE bool fileExists(QString path);

    // Themes
    Q_INVOKABLE QUrl getThemeIcon(const QString& name); //from custom theme dir or default if not found in the theme dir

    // Android picture capture
    Q_INVOKABLE QgsQuickPictureSource* getPicture( const QString &prefix );
    Q_INVOKABLE void open( const QString& data, const QString& type );

    // Singletons
    QgsQuickStyle* style() const; //if you want to access it from QML, use Style QML singleton

signals:
    void devicePixelsChanged();
    void themeDirChanged();

private:
    explicit QgsQuickUtils(QObject *parent = 0);
    ~QgsQuickUtils();

    static QgsQuickUtils* sInstance;

    //created and owned by the singleton
    QgsQuickStyle* mStyle;
    // created and owned by the sigleton to be able to use static functions
    QgsCoordinateReferenceSystem* mCoordinateReferenceSystem;

};

#endif // QGSQUICKUTILS_H
