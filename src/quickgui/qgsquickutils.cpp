/***************************************************************************
  qgsquickutils.cpp
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
#include <QDesktopWidget>
#include <QString>

#include "qgis.h"
#include "qgscoordinatereferencesystem.h"
#include "qgscoordinatetransform.h"
#include "qgsdistancearea.h"
#include "qgslogger.h"
#include "qgsvectorlayer.h"

#include "qgsquickmapsettings.h"
#include "qgsquickutils.h"
#include "qgsunittypes.h"


QgsQuickUtils *QgsQuickUtils::sInstance = 0;

QgsQuickUtils *QgsQuickUtils::instance()
{
  if ( !sInstance )
  {
    QgsDebugMsg( QStringLiteral( "QgsQuickUtils created: %1" ).arg( long( QThread::currentThreadId() ) ) );
    sInstance = new QgsQuickUtils();
  }
  return sInstance;
}

QgsQuickUtils::QgsQuickUtils( QObject *parent )
  : QObject( parent )
{

  // calculate screen density for calculation of real pixel sizes from density-independent pixels
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int dpi = dpiX < dpiY ? dpiX : dpiY; // In case of asymmetrical DPI. Improbable
  mScreenDensity = dpi / 160.;  // 160 DPI is baseline for density-independent pixels in Android
}

QgsQuickUtils::~QgsQuickUtils()
{
}

QgsCoordinateReferenceSystem QgsQuickUtils::coordinateReferenceSystemFromEpsgId( long epsg ) const
{
  return QgsCoordinateReferenceSystem::fromEpsgId( epsg );
}

double QgsQuickUtils::screenUnitsToMeters( QgsQuickMapSettings *mapSettings, int baseLengthPixels ) const
{
  if ( mapSettings == 0 ) return 0;

  QgsDistanceArea mDistanceArea;
  mDistanceArea.setEllipsoid( "WGS84" );
  mDistanceArea.setSourceCrs( mapSettings->destinationCrs(), mapSettings->transformContext() );

  // calculate the geographic distance from the central point of extent
  // to the specified number of points on the right side
  QSize s = mapSettings->outputSize();
  QPoint pointCenter( s.width() / 2, s.height() / 2 );
  QgsPointXY p1 = mapSettings->screenToCoordinate( pointCenter );
  QgsPointXY p2 = mapSettings->screenToCoordinate( pointCenter + QPoint( baseLengthPixels, 0 ) );
  return mDistanceArea.measureLine( p1, p2 );
}

void QgsQuickUtils::logMessage( const QString &message, const QString &tag, Qgis::MessageLevel level )
{
  QgsMessageLog::logMessage( message, tag, level );
}

QString QgsQuickUtils::dumpScreenInfo() const
{
  QRect rec = QApplication::desktop()->screenGeometry();
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int height = rec.height();
  int width = rec.width();
  double sizeX = ( double ) width / dpiX * 25.4;
  double sizeY = ( double ) height / dpiY * 25.4;

  QString msg;
  msg += "screen resolution: " + QString::number( width ) + "x" + QString::number( height ) + " px\n";
  msg += "screen DPI: " + QString::number( dpiX ) + "x" + QString::number( dpiY ) + "\n";
  msg += "screen size: " + QString::number( sizeX, 'f', 0 ) + "x" + QString::number( sizeY, 'f', 0 ) + " mm\n";
  msg += "screen density: " + QString::number( mScreenDensity );
  return msg;
}

qreal QgsQuickUtils::screenDensity() const
{
  return mScreenDensity;
}
