/***************************************************************************
 qgsquickpositionkit.cpp
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

#include "qgsquickpositionkit.h"

QgsQuickPositionKit::QgsQuickPositionKit(QObject *parent)
  : QObject(parent)
  , mAccuracy(-1)
  , mDirection(-1)
  , mHasPosition(false)
  , mIsSimulated(false)
  , mSource(nullptr)
{
  use_gps_location();
}

QGeoPositionInfoSource*  QgsQuickPositionKit::gpsSource() {
    // this should give us "true" position source
    // on Linux it comes from Geoclue library
    QGeoPositionInfoSource* source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source->error() != QGeoPositionInfoSource::NoError) {
        qDebug() << "Unable to create default QGeoPositionInfoSource: " << source->error();
        delete source;
        return 0;
    } else {
        return source;
    }
}

QGeoPositionInfoSource*  QgsQuickPositionKit::simulatedSource(double longitude, double latitude, double radius) {
    return new QgsQuickSimulatedPositionSource(this, longitude, latitude, radius);
}

void QgsQuickPositionKit::use_simulated_location(double longitude, double latitude, double radius) {
    QGeoPositionInfoSource* source = simulatedSource(longitude, latitude, radius);
    replacePositionSource(source);
}

void QgsQuickPositionKit::use_gps_location() {
    QGeoPositionInfoSource* source = gpsSource();
    replacePositionSource(source);
}

void QgsQuickPositionKit::replacePositionSource(QGeoPositionInfoSource* source) {
    if (mSource == source)
        return;

    if (mSource) {
        disconnect(mSource, 0, this, 0);
        delete mSource;
        mSource = 0;
    }

    mSource = source;

    if (mSource)
    {
      connect(mSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
              this, SLOT(positionUpdated(QGeoPositionInfo)));
      connect(mSource, SIGNAL(updateTimeout()), this, SLOT(onUpdateTimeout()));
      mSource->startUpdates();
      qDebug() << "Position source changed: " << mSource->sourceName();
    }
}

void QgsQuickPositionKit::positionUpdated(const QGeoPositionInfo &info)
{
  // qDebug() << "Position updated:" << info;
  mPosition = QgsPoint(info.coordinate().longitude(),
                       info.coordinate().latitude(),
                       info.coordinate().altitude());

  if (info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
    mAccuracy = info.attribute(QGeoPositionInfo::HorizontalAccuracy);
  else
    mAccuracy = -1;
  if (info.hasAttribute(QGeoPositionInfo::Direction))
    mDirection = info.attribute(QGeoPositionInfo::Direction);
  else
    mDirection = -1;
  //qDebug("acc %f dir %f", mAccuracy, mDirection);
  emit positionChanged();

  if (!mHasPosition)
  {
    mHasPosition = true;
    emit hasPositionChanged();
  }
}


void QgsQuickPositionKit::onUpdateTimeout()
{
  if (mHasPosition)
  {
    mHasPosition = false;
    emit hasPositionChanged();
  }
}

//////////////////////////

QgsQuickSimulatedPositionSource::QgsQuickSimulatedPositionSource(QObject *parent, double longitude, double latitude, double flightRadius)
  : QGeoPositionInfoSource(parent)
  , mTimer(new QTimer(this))
  , mAngle(0)
  , mFlightRadius(flightRadius)
  , mLongitude(longitude)
  , mLatitude(latitude)
{
  connect(mTimer, SIGNAL(timeout()), this, SLOT(readNextPosition()));
}

void QgsQuickSimulatedPositionSource::startUpdates()
{
  int interval = updateInterval();
  if (interval < minimumUpdateInterval())
    interval = minimumUpdateInterval();

  mTimer->start(interval);
}

void QgsQuickSimulatedPositionSource::stopUpdates()
{
  mTimer->stop();
}

void QgsQuickSimulatedPositionSource::requestUpdate(int /*timeout*/)
{
  readNextPosition();
  //emit updateTimeout(); // if the position would not be available
}

void QgsQuickSimulatedPositionSource::readNextPosition()
{
  double latitude = mLatitude, longitude = mLongitude;
  latitude += sin(mAngle*M_PI/180) * mFlightRadius;
  longitude += cos(mAngle*M_PI/180) * mFlightRadius;
  mAngle += 5;

  QDateTime timestamp = QDateTime::currentDateTime();
  QGeoCoordinate coordinate(latitude, longitude);
  QGeoPositionInfo info(coordinate, timestamp);
  if (info.isValid()) {
      mLastPosition = info;
      info.setAttribute(QGeoPositionInfo::Direction, 360 - int(mAngle)%360);
      int accuracy = std::rand() % 4000 + 8000; // rand accuracy <8000,12000>m
      info.setAttribute(QGeoPositionInfo::HorizontalAccuracy, accuracy);
      emit positionUpdated(info);
  }
}
