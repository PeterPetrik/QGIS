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

#include "qgsquickmapsettings.h"
#include "qgsquickutils.h"
#include "qgsunittypes.h"


QgsQuickUtils::QgsQuickUtils( QObject *parent )
  : QObject( parent )
  , mScreenDensity( calculateScreenDensity() )
{
}

/**
 * Makes QgsCoordinateReferenceSystem::fromEpsgId accessable for QML components
 */
QgsCoordinateReferenceSystem QgsQuickUtils::coordinateReferenceSystemFromEpsgId( long epsg ) const
{
  return QgsCoordinateReferenceSystem::fromEpsgId( epsg );
}

QgsPointXY QgsQuickUtils::pointXYFactory( double x, double y ) const
{
  return QgsPointXY( x, y );
}

QgsPoint QgsQuickUtils::pointFactory( double x, double y ) const
{
  return QgsPoint( x, y );
}

QgsPoint QgsQuickUtils::coordinateToPoint( const QGeoCoordinate &coor ) const
{
  return QgsPoint( coor.longitude(), coor.latitude(), coor.altitude() );
}

QgsPointXY QgsQuickUtils::transformPoint( const QgsCoordinateReferenceSystem &srcCrs,
    const QgsCoordinateReferenceSystem &destCrs,
    const QgsCoordinateTransformContext &context,
    const QgsPointXY &srcPoint ) const
{
  QgsCoordinateTransform mTransform( srcCrs, destCrs, context );
  QgsPointXY pt = mTransform.transform( srcPoint );
  return pt;
}

double QgsQuickUtils::screenUnitsToMeters( QgsQuickMapSettings *mapSettings, int baseLengthPixels ) const
{
  if ( mapSettings == nullptr ) return 0.0;

  QgsDistanceArea mDistanceArea;
  mDistanceArea.setEllipsoid( QStringLiteral( "WGS84" ) );
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

QUrl QgsQuickUtils::getThemeIcon( const QString &name )
{
  QString extension( ".svg" );
  QString path = "qrc:/" + name + extension;
  QgsDebugMsg( QStringLiteral( "Using icon %1 from %2" ).arg( name, path ) );
  return QUrl( path );
}

QString QgsQuickUtils::qgsPointToString( const QgsPoint &point, int decimals )
{
  QString label;
  label += QString::number( point.x(), 'f', decimals );
  label += ", ";
  label += QString::number( point.y(), 'f', decimals );
  return label;
}

QString QgsQuickUtils::distanceToString( qreal dist, int decimals )
{
  if ( dist < 0 )
  {
    return "0 m";
  }

  QString label;
  if ( dist > 1000 )
  {
    label += QString::number( dist / 1000.0, 'f', decimals );
    label += QString( " km" );
  }
  else
  {
    if ( dist > 1 )
    {
      label += QString::number( dist, 'f', decimals );
      label += QString( " m" );
    }
    else
    {
      label += QString::number( dist * 1000, 'f', decimals );
      label += QString( " mm" );
    }
  }
  return label;
}

QString QgsQuickUtils::dumpScreenInfo() const
{
  QRect rec = QApplication::desktop()->screenGeometry();
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int height = rec.height();
  int width = rec.width();
  double sizeX = static_cast<double>( width ) / dpiX * 25.4;
  double sizeY = static_cast<double>( height ) / dpiY * 25.4;

  QString msg;
  msg += tr( "screen resolution: %1x%2 px\n" ).arg( width ).arg( height );
  msg += tr( "screen DPI: %1x%2\n" ).arg( dpiX ).arg( dpiY );
  msg += tr( "screen size: %1x%2 mm\n" ).arg( QString::number( sizeX, 'f', 0 ), QString::number( sizeY, 'f', 0 ) );
  msg += tr( "screen density: %1" ).arg( mScreenDensity );
  return msg;
}

qreal QgsQuickUtils::screenDensity() const
{
  return mScreenDensity;
}

qreal QgsQuickUtils::calculateScreenDensity()
{
  // calculate screen density for calculation of real pixel sizes from density-independent pixels
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int dpi = dpiX < dpiY ? dpiX : dpiY; // In case of asymmetrical DPI. Improbable
  return dpi / 160.;  // 160 DPI is baseline for density-independent pixels in Android
}
