/***************************************************************************
     testqgsclipper.cpp
     --------------------------------------
    Date                 : Tue 14 Aug 2012
    Copyright            : (C) 2012 by Magnus Homann
    Email                : magnus at homann dot se
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qgstest.h"
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QString>
#include <QStringList>
#include <qgsapplication.h>
#include "qgspoint.h"
#include "qgspointxy.h"
#include "qgscoordinatereferencesystem.h"

//header for class being tested
#include <qgsquickutils.h>

class TestQgsQuickUtils: public QObject
{
    Q_OBJECT
  private slots:
    void init() {} // will be called before each testfunction is executed.
    void cleanup() {} // will be called after every testfunction.

    void crs_and_geometry();
};

void TestQgsQuickUtils::crs_and_geometry()
{
  QgsCoordinateReferenceSystem crs3857 = QgsQuickUtils::instance()->coordinateReferenceSystemFromEpsgId(3857);
  QVERIFY( crs3857.authid() == "EPSG:3857" );

  QgsCoordinateReferenceSystem crsGPS = QgsQuickUtils::instance()->coordinateReferenceSystemFromEpsgId(4326);
  QVERIFY( crsGPS.authid() == "EPSG:4326" );

  QgsPointXY pointXY = QgsQuickUtils::instance()->pointXYFactory(49.9, 16.3);
  QVERIFY( pointXY.x() == 49.9 );
  QVERIFY( pointXY.y() == 16.3 );

  QgsPoint point = QgsQuickUtils::instance()->pointFactory(1.0, -1.0);
  QVERIFY( point.x() == 1.0 );
  QVERIFY( point.y() == -1.0 );


  QgsPointXY transformedPoint = QgsQuickUtils::instance()->transformPoint(crsGPS, crs3857, pointXY);
  QVERIFY( fabs(transformedPoint.x() - 5554843) < 1.0 );
  QVERIFY( fabs(transformedPoint.y() - 1839491) < 1.0 );

  //Q_INVOKABLE double screenUnitsToMeters(QgsQuickMapSettings* mapSettings, int baseLengthPixels) const;
}

QGSTEST_MAIN( TestQgsQuickUtils )
#include "testqgsquickutils.moc"
