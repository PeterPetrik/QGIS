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

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QThread>

#include "qgscoordinatereferencesystem.h"
#include "qgscoordinatetransform.h"
#include "qgsdistancearea.h"
#include "qgsmessagelog.h"
#include "qgsvectorlayer.h"

#include "qgsquickmapsettings.h"
#include "qgsquickutils.h"


QgsQuickUtils *QgsQuickUtils::sInstance = 0;

QgsQuickUtils *QgsQuickUtils::instance()
{
  if ( !sInstance )
  {
    qDebug() << "QgsQuickUtils created: " << QThread::currentThreadId();
    sInstance = new QgsQuickUtils();
  }
  return sInstance;
}

QgsQuickUtils::QgsQuickUtils( QObject *parent ):
  QObject( parent )
{
  mCoordinateReferenceSystem = new QgsCoordinateReferenceSystem();
}

QgsQuickUtils::~QgsQuickUtils()
{

  if ( mCoordinateReferenceSystem )
  {
    delete mCoordinateReferenceSystem;
    mCoordinateReferenceSystem = 0;
  }
}

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


QgsPointXY QgsQuickUtils::transformPoint( const QgsCoordinateReferenceSystem &srcCrs,
    const QgsCoordinateReferenceSystem &destCrs,
    const QgsCoordinateTransformContext &context,
    const QgsPointXY &srcPoint ) const
{
  QgsCoordinateTransform mTransform( srcCrs, destCrs, context );
  QgsPointXY pt = mTransform.transform( srcPoint );
  return pt;
}

bool QgsQuickUtils::hasValidGeometry( QgsVectorLayer *layer, const QgsFeature &feat )
{
  if ( !layer )
    return false;

  if ( !feat.hasGeometry() )
    return false;

  if ( feat.geometry().type() != layer->geometryType() )
    return false;

  if ( QgsWkbTypes::hasZ( layer->wkbType() ) != QgsWkbTypes::hasZ( feat.geometry().wkbType() ) )
    return false;

  return true;
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

bool QgsQuickUtils::fileExists( QString path )
{
  QFileInfo check_file( path );
  // check if file exists and if yes: Is it really a file and no directory?
  if ( check_file.exists() && check_file.isFile() )
  {
    return true;
  }
  else
  {
    return false;
  }
}

void QgsQuickUtils::copyFile( QString sourcePath, QString targetPath )
{
  if ( !fileExists( sourcePath ) )
  {
    qDebug() << "Source file does not exist!" << sourcePath;
    return;
  }

  if ( !QDir::root().mkpath( targetPath ) )
  {
    QgsApplication::messageLog()->logMessage( tr( "Could not create folder %1" ).arg( targetPath ), "QgsQuick", QgsMessageLog::CRITICAL );
    return;
  }

  QDir dir( targetPath );
  QString filename( QFile( sourcePath ).fileName() );

  if ( !QFile( sourcePath ).rename( dir.absoluteFilePath( filename ) ) )
  {
    qDebug() << "Couldn't rename file! Trying to copy instead";
    if ( !QFile( sourcePath ).copy( dir.absoluteFilePath( filename ) ) )
    {
      QgsApplication::messageLog()->logMessage( tr( "File %1 could not be copied to folder %2.", "QgsQuick", QgsMessageLog::CRITICAL ).arg( sourcePath, targetPath ) );
      return;
    }
  }
}

void QgsQuickUtils::remove( QString path )
{
  QFile::remove( path );
}

QString QgsQuickUtils::getFileName( QString path )
{
  QFileInfo fileInfo( path );
  QString filename( fileInfo.fileName() );
  return filename;
}

void QgsQuickUtils::logMessage( const QString &message, const QString &tag, QgsMessageLog::MessageLevel level )
{
  QgsMessageLog::logMessage( message, tag, level );
}

QUrl QgsQuickUtils::getThemeIcon( const QString &name )
{
  QString extension( ".svg" );
  QString path = "qrc:/" + name + extension;
  qDebug() << "Using icon " << name << " from " << path;
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

qreal QgsQuickUtils::devicePixels() const
{
  return mDevicePixels;
}

void QgsQuickUtils::setDevicePixels( qreal dp )
{
    // Q_ASSERT( !mStyle && "Style must be assigned only once!" );
  if ( dp != mDevicePixels )
  {
    mDevicePixels = dp;
    emit devicePixelsChanged();
  }
}
