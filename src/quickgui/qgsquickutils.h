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


#include <QObject>
#include <QString>
#include <QUrl>

#include "qgsfeature.h"
#include "qgsmessagelog.h"
#include "qgspoint.h"
#include "qgspointxy.h"

#include "qgsquickmapsettings.h"
#include "qgis_quick.h"


class QgsFeature;
class QgsVectorLayer;
class QgsQuickStyle;
class QgsCoordinateReferenceSystem;

/**
 * \ingroup quick
 *
 * Singleton encapsulating the common utilies for QgsQuick library.
 *
 * \note QML Type: Style (singleton)
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickUtils: public QObject
{
  Q_OBJECT

  public:
    static QgsQuickUtils* instance();

    // CRS and geometry

    /**
      * Create crs from epsg code in QML
      */
    Q_INVOKABLE QgsCoordinateReferenceSystem coordinateReferenceSystemFromEpsgId(long epsg) const;

    /**
      * Create QgsPointXY in QML
      */
    Q_INVOKABLE QgsPointXY pointXYFactory(double x, double y) const;

    /**
      * Create QgsPoint in QML
      */
    Q_INVOKABLE QgsPoint pointFactory(double x, double y) const;

    /**
      * Transform point between different crs from QML
      */
    Q_INVOKABLE QgsPointXY transformPoint(QgsCoordinateReferenceSystem srcCrs, QgsCoordinateReferenceSystem destCrs, QgsPointXY srcPoint) const;

    /**
      * Calculate the distance in meter representing baseLengthPixels pixels on the screen based on the current map settings.
      */
    Q_INVOKABLE double screenUnitsToMeters(QgsQuickMapSettings* mapSettings, int baseLengthPixels) const;

    /**
      * Has QgsFeature a geometry that can be added to the layer (non-emptry, same geometry type)?
      */
    Q_INVOKABLE bool hasValidGeometry(QgsVectorLayer* layer, const QgsFeature& feat);

    // Common
    Q_INVOKABLE bool fileExists(QString path);
    Q_INVOKABLE void copyFile(QString sourcePath, QString targetPath);
    Q_INVOKABLE void remove(QString path);
    Q_INVOKABLE QString getFileName(QString path);
    Q_INVOKABLE void logMessage( const QString &message,
                                 const QString &tag = QString("QgsQuick"),
                                 QgsMessageLog::MessageLevel level = QgsMessageLog::WARNING );

    // Themes
    /**
      * Get icon from custom theme dir or default if not found in the theme dir
      *
      * Android 4.4 (API level 20) and lower doesn't support vector drawables (svg)
      * https://developer.android.com/studio/write/vector-asset-studio.html
      * Set useVectorIcons in Style to use SVGs instead of PNGs.
      *
      */
    Q_INVOKABLE QUrl getThemeIcon(const QString& name);

    // Formatting
    /**
      * point to string, e.g. -2.234521, 34.4444421 -> -2.234, 34.444
      */
    Q_INVOKABLE QString qgsPointToString(const QgsPoint& point, int decimals = 3);
    /**
      * distance in meters to human readable length e.g. 1222.234 m -> 1.2 km
      */
    Q_INVOKABLE QString distanceToString(qreal dist, int decimals = 1);


    // Singletons
    void setStyle( QgsQuickStyle *style );
    QgsQuickStyle* style() const; //if you want to access it from QML, use Style QML singleton

signals:
    void devicePixelsChanged();
    void themeDirChanged();

private:
    explicit QgsQuickUtils(QObject *parent = 0);
    ~QgsQuickUtils();

    static QgsQuickUtils* sInstance;

    // style - not owned by this class
    QgsQuickStyle* mStyle = nullptr;
    // created and owned by the sigleton to be able to use static functions
    QgsCoordinateReferenceSystem* mCoordinateReferenceSystem;

};

#endif // QGSQUICKUTILS_H
