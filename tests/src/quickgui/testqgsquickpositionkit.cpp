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

#include "qgsquickpositionkit.h"
#include "qgsquicksimulatedpositionsource.h"
#include "qgsquickutils.h"

class TestQgsQuickUtils: public QObject
{
    Q_OBJECT
  private slots:
    void init() {} // will be called before each testfunction is executed.
    void cleanup() {} // will be called after every testfunction.

    void screen_density();

  private:
    QgsQuickUtils utils;
    QgsQuickPositionKit positionKit;
};

void TestQgsQuickUtils::screen_density()
{

  QVERIFY(!positionKit.simulated());
  positionKit.use_simulated_location(-97.36, 36.93, 2);
  QVERIFY(positionKit.simulated());

  QVERIFY(positionKit.hasPosition());
  QVERIFY(positionKit.accuracy() > 0);
}

QGSTEST_MAIN( TestQgsQuickUtils )
#include "testqgsquickpositionkit.moc"
