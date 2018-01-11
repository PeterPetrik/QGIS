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

#include "qgis_quick.h"
#include <QObject>
#include <QtPositioning>
#include <qgspoint.h>

class QUICK_EXPORT QgsQuickPositionKit : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QgsPoint position READ position NOTIFY positionChanged) // in WGS84 coords
  Q_PROPERTY(bool hasPosition READ hasPosition NOTIFY hasPositionChanged)
  Q_PROPERTY(qreal accuracy READ accuracy NOTIFY positionChanged) // in meters
  Q_PROPERTY(qreal direction READ direction NOTIFY positionChanged)
  //TODO add timestamp?
  Q_PROPERTY(bool isSimulated READ simulated NOTIFY isSimulatedChanged)

public:
  explicit QgsQuickPositionKit(QObject *parent = 0);

  bool hasPosition() const { return mHasPosition; }
  QgsPoint position() const { return mPosition; }
  qreal accuracy() const { return mAccuracy; }
  qreal direction() const { return mDirection; }

  // We do not want to have the origin point as property
  // We basically want to set it once based on project/map cente and keep
  // it that way regardless of mapsettings change (e.g. zoom etc)
  Q_INVOKABLE void use_simulated_location(double longitude, double latitude, double radius);
  Q_INVOKABLE void use_gps_location();
  bool simulated() const { return mIsSimulated; }

signals:
  void positionChanged();
  void hasPositionChanged();
  void isSimulatedChanged();
  void statusChanged();

public slots:

private slots:
    void positionUpdated(const QGeoPositionInfo &info);
    void onUpdateTimeout();

protected:


protected:
  QgsPoint mPosition; // in WGS84 coords
  qreal mAccuracy; // horizontal accuracy in meters (-1 if not available)
  qreal mDirection; // bearing in degrees clockwise from north to direction of travel (-1 if not available)
  bool mHasPosition;

  // Simulated source
  bool mIsSimulated;

  QGeoPositionInfoSource* mSource;

private:
  void replacePositionSource(QGeoPositionInfoSource* source);
  QString calculateStatusLabel();

  QGeoPositionInfoSource* gpsSource();
  QGeoPositionInfoSource* simulatedSource(double longitude, double latitude, double radius);

};

/** simulated GPS - makes circles around a fixed position */
class QUICK_NO_EXPORT QgsQuickSimulatedPositionSource : public QGeoPositionInfoSource
{
    Q_OBJECT
public:
    QgsQuickSimulatedPositionSource(QObject *parent, double longitude, double latitude, double flightRadius);

    QGeoPositionInfo lastKnownPosition(bool /*fromSatellitePositioningMethodsOnly = false*/) const { return mLastPosition; }
    PositioningMethods supportedPositioningMethods() const { return AllPositioningMethods; }
    int minimumUpdateInterval() const { return 1000; }
    Error error() const { return QGeoPositionInfoSource::NoError; }

public slots:
    virtual void startUpdates();
    virtual void stopUpdates();

    virtual void requestUpdate(int timeout = 5000);

private slots:
    void readNextPosition();

private:
    QTimer *mTimer;
    QGeoPositionInfo mLastPosition;
    double mAngle;

    double mFlightRadius;
    double mLongitude;
    double mLatitude;
};

#endif // QGSQUICKPOSITIONKIT_H
