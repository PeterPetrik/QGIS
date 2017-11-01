/***************************************************************************
  qgsquickmapsettings.cpp
  --------------------------------------
  Date                 : 27.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickmapsettings.h"

#include <qgsmaplayer.h>
#include <qgsproject.h>
#include <qgsmessagelog.h>

#include <qgsmaplayerstylemanager.h>


QgsQuickMapSettings::QgsQuickMapSettings( QObject* parent )
  : QObject( parent )
{
  // Connect signals for derived values
  connect( this, &QgsQuickMapSettings::destinationCrsChanged, this, &QgsQuickMapSettings::mapUnitsPerPixelChanged );
  connect( this, &QgsQuickMapSettings::extentChanged, this, &QgsQuickMapSettings::mapUnitsPerPixelChanged );
  connect( this, &QgsQuickMapSettings::outputSizeChanged, this, &QgsQuickMapSettings::mapUnitsPerPixelChanged );
  connect( this, &QgsQuickMapSettings::extentChanged, this, &QgsQuickMapSettings::visibleExtentChanged );
  connect( this, &QgsQuickMapSettings::rotationChanged, this, &QgsQuickMapSettings::visibleExtentChanged );
  connect( this, &QgsQuickMapSettings::outputSizeChanged, this, &QgsQuickMapSettings::visibleExtentChanged );
  connect( QgsProject::instance(), &QgsProject::readProject, this, &QgsQuickMapSettings::onReadProject );
}

QgsQuickMapSettings::~QgsQuickMapSettings()
{

}

QgsRectangle QgsQuickMapSettings::extent() const
{
  return mMapSettings.extent();
}

void QgsQuickMapSettings::setExtent( const QgsRectangle& extent )
{
  if ( mMapSettings.extent() == extent )
    return;

  mMapSettings.setExtent( extent );
  emit extentChanged();
}

void QgsQuickMapSettings::setCenter( const QgsPoint& center )
{
  QgsVector delta = QgsPointXY( center ) - mMapSettings.extent().center();

  QgsRectangle e = mMapSettings.extent();
  e.setXMinimum( e.xMinimum() + delta.x() );
  e.setXMaximum( e.xMaximum() + delta.x() );
  e.setYMinimum( e.yMinimum() + delta.y() );
  e.setYMaximum( e.yMaximum() + delta.y() );

  setExtent( e );
}

double QgsQuickMapSettings::mapUnitsPerPixel() const
{
  return mMapSettings.mapUnitsPerPixel();
}

QgsRectangle QgsQuickMapSettings::visibleExtent() const
{
  return mMapSettings.visibleExtent();
}

QPointF QgsQuickMapSettings::coordinateToScreen( const QgsPoint& p ) const
{
  QgsPointXY pt( p.x(), p.y() );
  QgsPointXY pp = mMapSettings.mapToPixel().transform( pt );
  return pp.toQPointF();
}

QgsPoint QgsQuickMapSettings::screenToCoordinate( const QPointF& p ) const
{
  const QgsPointXY pp = mMapSettings.mapToPixel().toMapCoordinates( p.toPoint() );
  return QgsPoint( pp );
}

QgsMapSettings QgsQuickMapSettings::mapSettings() const
{
  return mMapSettings;
}

QSize QgsQuickMapSettings::outputSize() const
{
  return mMapSettings.outputSize();
}

void QgsQuickMapSettings::setOutputSize( const QSize& outputSize )
{
  if ( mMapSettings.outputSize() == outputSize )
    return;

  mMapSettings.setOutputSize( outputSize );
  emit outputSizeChanged();
}

double QgsQuickMapSettings::outputDpi() const
{
  return mMapSettings.outputDpi();
}

void QgsQuickMapSettings::setOutputDpi( double outputDpi )
{
  if ( mMapSettings.outputDpi() == outputDpi )
    return;

  mMapSettings.setOutputDpi( outputDpi );
  emit outputDpiChanged();
}

QgsCoordinateReferenceSystem QgsQuickMapSettings::destinationCrs() const
{
  return mMapSettings.destinationCrs();
}

void QgsQuickMapSettings::setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs )
{
  if ( mMapSettings.destinationCrs() == destinationCrs )
    return;

  mMapSettings.setDestinationCrs( destinationCrs );
  emit destinationCrsChanged();
}

QList<QgsMapLayer*> QgsQuickMapSettings::layers() const
{
  return mMapSettings.layers();
}

void QgsQuickMapSettings::setLayers( const QList<QgsMapLayer*>& layers )
{
  mMapSettings.setLayers( layers );
  emit layersChanged();
}

#if 0
void MapSettings::setMapTheme( QgsProject* project, const QString& mapThemeName )
{
  QStringList layerIds;

  QgsMapThemeCollection::MapThemeRecord mapTheme = project->mapThemeCollection()->mapThemeState( mapThemeName );

  Q_FOREACH( const QgsMapThemeCollection::MapThemeLayerRecord& record, mapTheme.layerRecords() )
  {
    record.layer()->styleManager()->setCurrentStyle( mapTheme.perLayerCurrentStyle().value( layerId ) );

    layerIds << layerId;
  }
}

mMapSettings.setLayers( layerIds );
emit layersChanged();
}
#endif

void QgsQuickMapSettings::onReadProject( const QDomDocument& doc )
{
  QDomNodeList nodes = doc.elementsByTagName( "mapcanvas" );
  if ( nodes.count() )
  {
    QDomNode node = nodes.item( 0 );

    mMapSettings.readXml( node );

    if ( mMapSettings.rotation() != 0 )
      QgsMessageLog::logMessage( tr( "Map Canvas rotation is not supported. Resetting from %1 to 0." ).arg( mMapSettings.rotation() ) );

    mMapSettings.setRotation( 0 );

    emit extentChanged();
    emit destinationCrsChanged();
    emit outputSizeChanged();
    emit outputDpiChanged();
    emit layersChanged();
  }
}

double QgsQuickMapSettings::rotation() const
{
  return mMapSettings.rotation();
}

void QgsQuickMapSettings::setRotation( double rotation )
{
  if ( rotation == mMapSettings.rotation() )
    return;

  mMapSettings.setRotation( rotation );
  emit rotationChanged();
}
