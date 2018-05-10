/***************************************************************************
     testqgsquickutils.cpp
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
#include <QObject>
#include <QApplication>
#include <QDesktopWidget>

#include "qgsapplication.h"
#include "qgstest.h"
#include "qgis.h"

#include "qgsquickutils.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickscalebarkit.h"

class TestQgsQuickScaleBarKit: public QObject
{
    Q_OBJECT
  private slots:
    void init() {} // will be called before each testfunction is executed.
    void cleanup() {} // will be called after every testfunction.

    void updateScaleBar();
};

void TestQgsQuickScaleBarKit::updateScaleBar()
{
  QgsCoordinateReferenceSystem crsGPS = QgsQuickUtils::instance()->coordinateReferenceSystemFromEpsgId( 4326 );
  QVERIFY( crsGPS.authid() == "EPSG:4326" );

  QgsRectangle extent = QgsRectangle( 49, 16, 50, 17 );
  QgsQuickMapCanvasMap canvas;

  QgsQuickMapSettings *ms = canvas.mapSettings();
  ms->setDestinationCrs( crsGPS );
  ms->setExtent( extent );
  ms->setOutputSize( QSize( 1000, 500 ) );

  QgsQuickScaleBarKit kit;
  kit.setMapSettings( ms );
  QVERIFY( kit.units() == QString( "km" ) );

  qreal scale = 0.000005;
  canvas.zoom( extent.center().toQPointF(), scale );
  QVERIFY( kit.units() == QString( "m" ) );
}

QGSTEST_MAIN( TestQgsQuickScaleBarKit )
#include "testqgsquickscalebarkit.moc"
