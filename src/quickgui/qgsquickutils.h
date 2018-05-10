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

#include "qgis.h"
#include "qgsmessagelog.h"
#include "qgsquickmapsettings.h"
#include "qgis_quick.h"

class QgsCoordinateReferenceSystem;
/**
 * \ingroup quick
 *
 * Singleton encapsulating the common utilies for QgsQuick library.
 *
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
      */
    Q_PROPERTY( qreal dp READ screenDensity CONSTANT )

  public:
    //! return instance of the QgsQuickUtils singleton
    static QgsQuickUtils *instance();

    //! Calculated density of the screen - see "dp" property for more details
    qreal screenDensity() const;

    /**
      * Create crs from epsg code in QML
      */
    Q_INVOKABLE QgsCoordinateReferenceSystem coordinateReferenceSystemFromEpsgId( long epsg ) const;
    /**
      * Calculate the distance in meter representing baseLengthPixels pixels on the screen based on the current map settings.
      */
    Q_INVOKABLE double screenUnitsToMeters( QgsQuickMapSettings *mapSettings, int baseLengthPixels ) const;

//! Log message in QgsMessageLog
    Q_INVOKABLE void logMessage( const QString &message,
                                 const QString &tag = QString( "QgsQuick" ),
                                 Qgis::MessageLevel level = Qgis::Warning );

//! Returns a string with information about screen size and resolution - useful for debugging
    QString dumpScreenInfo() const;


  private:
    explicit QgsQuickUtils( QObject *parent = nullptr );
    ~QgsQuickUtils();

    static QgsQuickUtils *sInstance;

    qreal mScreenDensity;

};

#endif // QGSQUICKUTILS_H
