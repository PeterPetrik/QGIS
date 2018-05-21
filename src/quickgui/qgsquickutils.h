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
#include <QtPositioning/QGeoCoordinate>

#include "qgis.h"
#include "qgsmessagelog.h"
#include "qgspoint.h"
#include "qgspointxy.h"

#include "qgsquickmapsettings.h"
#include "qgis_quick.h"

class QgsCoordinateReferenceSystem;

/**
 * \ingroup quick
 *
 * Encapsulating the common utilies for QgsQuick library.
 *
 * \note QML Type: Utils (Singleton)
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickUtils: public QObject
{
    Q_OBJECT

    /**
      * "dp" is useful for building building components that work well with different screen densities.
      * It stands for density-independent pixels. A width of 10dp is going to be the same physical size
      * on all screens regardless their density. In QML code, all values are specified in screen pixels,
      * so in order to set a width of 10dp, one would use the following code: "width: 10 * QgsQuick.Utils.dp"
      *
      * 1dp is approximately 0.16mm. When screen has 160 DPI (baseline), the value of "dp" is 1.
      * On high DPI screen the value will be greater, e.g. 1.5.
      *
      * This is a readonly property.
      */
    Q_PROPERTY( qreal dp READ screenDensity CONSTANT )

  public:
    //! Create new utilities
    QgsQuickUtils( QObject *parent = nullptr );
    //! Destructor
    ~QgsQuickUtils() = default;

    //! \copydoc QgsQuickUtils::dp
    qreal screenDensity() const;

    /**
      * Creates crs from epsg code in QML
      */
    Q_INVOKABLE static QgsCoordinateReferenceSystem coordinateReferenceSystemFromEpsgId( long epsg );

    /**
      * Creates QgsPointXY in QML
      */
    Q_INVOKABLE QgsPointXY pointXYFactory( double x, double y ) const;

    /**
      * Creates QgsPoint in QML
      */
    Q_INVOKABLE QgsPoint pointFactory( double x, double y ) const;

    /**
      * Converts QGeoCoordinate to QgsPoint
      */
    Q_INVOKABLE QgsPoint coordinateToPoint( const QGeoCoordinate &coor ) const;

    /**
      * Transforms point between different crs from QML
      */
    Q_INVOKABLE static QgsPointXY transformPoint( const QgsCoordinateReferenceSystem &srcCrs,
        const QgsCoordinateReferenceSystem &destCrs,
        const QgsCoordinateTransformContext &context,
        const QgsPointXY &srcPoint );

    /**
      * Calculates the distance in meter representing baseLengthPixels pixels on the screen based on the current map settings.
      */
    Q_INVOKABLE static double screenUnitsToMeters( QgsQuickMapSettings *mapSettings, int baseLengthPixels );

    //! Log message in QgsMessageLog
    Q_INVOKABLE void logMessage( const QString &message,
                                 const QString &tag = QString( "QgsQuick" ),
                                 Qgis::MessageLevel level = Qgis::Warning );

    /**
      * Returns QUrl to image from libary's /images folder.
      */
    Q_INVOKABLE QUrl getThemeIcon( const QString &name );

    /**
      * Converts point to string with given decimals (default decimals = 3),
      * e.g. -2.234521, 34.4444421 -> -2.234, 34.444
      */
    Q_INVOKABLE static QString qgsPointToString( const QgsPoint &point, int decimals = 3 );

    /**
      * Converts distance in meters to human readable length with given decimals (default decimals = 1),
      *  e.g. 1222.234 m -> 1.2 km
      */
    Q_INVOKABLE static QString distanceToString( qreal dist, int decimals = 1 );

    //! Returns a string with information about screen size and resolution - useful for debugging
    QString dumpScreenInfo() const;

  private:
    static qreal calculateScreenDensity();

    qreal mScreenDensity;
};

#endif // QGSQUICKUTILS_H
