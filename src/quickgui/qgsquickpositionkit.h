/***************************************************************************
 qgsquickpositionkit.h
  --------------------------------------
  Date                 : Dec. 2017
  Copyright            : (C) 2017 Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKPOSITIONKIT_H
#define QGSQUICKPOSITIONKIT_H

#include <QObject>
#include <QtPositioning>

#include "qgspoint.h"

#include "qgis_quick.h"
#include "qgsquickmapsettings.h"

/**
 * \ingroup quick
 * Convenient set of tools to read GPS position and accuracy.
 *
 * Also, if one can use use_simulated_location to specify simulated position.
 * Simulated position source generates random points in circles around the selected
 * point and radius. Real GPS position is not used in this mode.
 *
 * \note QML Type: PositionKit
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickPositionKit : public QObject
{
    Q_OBJECT

    /**
     * GPS position in WGS84 coords
     */
    Q_PROPERTY( QgsPoint position READ position NOTIFY positionChanged )

    /**
     * GPS position is available (position property is a valid number)
     */
    Q_PROPERTY( bool hasPosition READ hasPosition NOTIFY hasPositionChanged )

    /**
     * GPS horizontal accuracy in meters, -1 if not available
     */
    Q_PROPERTY( qreal accuracy READ accuracy NOTIFY positionChanged )

    /**
     * GPS direction, bearing in degrees clockwise from north to direction of travel. -1 if not available
     */
    Q_PROPERTY( qreal direction READ direction NOTIFY positionChanged )

    /**
     * GPS position and accuracy is simulated (not real from GPS sensor). Default false (use real GPS)
     */
    Q_PROPERTY( bool isSimulated READ simulated NOTIFY isSimulatedChanged )

  public:
    //! Create new position kit
    explicit QgsQuickPositionKit( QObject *parent = 0 );

    //! True if there is \copydoc QgsQuickPositionKit::position
    bool hasPosition() const;

    //! \copydoc QgsQuickPositionKit::position
    QgsPoint position() const;

    //! \copydoc QgsQuickPositionKit::accuracy
    qreal accuracy() const;

    //! \copydoc QgsQuickPositionKit::direction
    qreal direction() const;

    //! \copydoc QgsQuickPositionKit::isSimulated
    bool simulated() const;

    /**
     * Use simulated GPS source.
     *
     * Simulated GPS source emulates point on circle around defined point in specified radius
     *
     * We do not want to have the origin point as property
     * We basically want to set it once based on project/map cente and keep
     * it that way regardless of mapsettings change (e.g. zoom etc)
     *
     * \param longitude longitude of the centre of the emulated points
     * \param latitude latitude of the centre of the emulated points
     * \param radius distance of emulated points from the centre (in degrees WSG84)
     */
    Q_INVOKABLE void use_simulated_location( double longitude, double latitude, double radius );

    /**
     * Generates label for gps accuracy according set gps source.
     * \param withAccuracy If False, returns empty string.
     * \param altMsg If there is no position, returns altMsg.
     */
    Q_INVOKABLE QString gpsAccuracyLabel( bool withAccuracy, QString altMsg );

    /**
     * Generates label for gps position according set gps source.
     * \param precision Defines number of digits after comma.
     * \param altMsg If there is no position, returns altMsg.
     */
    Q_INVOKABLE QString gpsPositionLabel( int precision, QString altMsg );

    /**
     * Use real GPS source (not simulated)
     */
    Q_INVOKABLE void use_gps_location();

    /**
     * Used for changing gps source when simulatePositionLongLatRad is un/set.
     * \param simulatePositionLongLatRad Vector containing longitute, latitute and radius.
     */
    Q_INVOKABLE void onSimulatePositionLongLatRadChanged( QVector<double> simulatePositionLongLatRad );

    /**
     * Calculates accuracy indicator width.
     * \param mapSettings QgsQuickMapSettings used for screenUnitsToMeters calculation.
     */
    Q_INVOKABLE double accuracyIndicatorWidth( QgsQuickMapSettings *mapSettings );

  signals:
    //! GPS position changed
    void positionChanged();

    //! hasPosition changed
    void hasPositionChanged();

    //! changed if GPS position is simulated or not
    void isSimulatedChanged();

  public slots:

  private slots:
    void positionUpdated( const QGeoPositionInfo &info );
    void onUpdateTimeout();

  protected:
    //! \copydoc QgsQuickPositionKit::position
    QgsPoint mPosition;
    //! \copydoc QgsQuickPositionKit::accuracy
    qreal mAccuracy;
    //! \copydoc QgsQuickPositionKit::direction
    qreal mDirection;
    //! \copydoc QgsQuickPositionKit::position
    bool mHasPosition;

    // Simulated source
    bool mIsSimulated;

    QGeoPositionInfoSource *mSource = nullptr;

  private:
    void replacePositionSource( QGeoPositionInfoSource *source );
    QString calculateStatusLabel();

    QGeoPositionInfoSource *gpsSource();
    QGeoPositionInfoSource *simulatedSource( double longitude, double latitude, double radius );

};

#endif // QGSQUICKPOSITIONKIT_H
