/***************************************************************************
  qgsquickidentifytool.cpp
 ---------------------
  Date                 : 30.8.2016
  Copyright            : (C) 2016 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickidentifykit.h"

#include "qgsquickproject.h"
#include "qgsquickmapsettings.h"
//#include "multifeaturelistmodel.h"

#include <qgsvectorlayer.h>
#include <qgsproject.h>
#include <qgsrenderer.h>

QgsQuickIdentifyKit::QgsQuickIdentifyKit( QObject *parent )
  : QObject( parent )
  , mMapSettings( nullptr )
  , mSearchRadiusMm( 8 )
{

}

QgsQuickMapSettings* QgsQuickIdentifyKit::mapSettings() const
{
  return mMapSettings;
}

void QgsQuickIdentifyKit::setMapSettings( QgsQuickMapSettings* mapSettings )
{
  if ( mapSettings == mMapSettings )
    return;

  mMapSettings = mapSettings;
  emit mapSettingsChanged();
}

void QgsQuickIdentifyKit::identify( const QPointF& point ) const
{
  //if ( !mModel || !mMapSettings )
  if ( !mMapSettings )
  {
    qWarning() << "Unable to use IdentifyTool without mapSettings or model property set.";
    return;
  }

  // mModel->clear();

  QgsPointXY mapPoint = mMapSettings->mapSettings().mapToPixel().toMapCoordinates( point.toPoint() );

  QStringList noIdentifyLayerIdList;
  if (mMapSettings->project()) {
    noIdentifyLayerIdList = mMapSettings->project()->project()->nonIdentifiableLayers();
  }

  Q_FOREACH( QgsMapLayer* layer, mMapSettings->mapSettings().layers() )
  {
    if (mMapSettings->project() && noIdentifyLayerIdList.contains( layer->id() ) )
      continue;

    QgsVectorLayer* vl = qobject_cast<QgsVectorLayer*>( layer );
    if ( vl )
    {
      QList<IdentifyResult> results = identifyVectorLayer( vl, mapPoint );

      // mModel->appendFeatures( results );
    }
  }
}

QList<QgsQuickIdentifyKit::IdentifyResult> QgsQuickIdentifyKit::identifyVectorLayer ( QgsVectorLayer* layer, const QgsPointXY& point ) const
{
  QList<IdentifyResult> results;

  if ( !layer || !layer->isSpatial() )
    return results;

  if ( !layer->isInScaleRange( mMapSettings->mapSettings().scale() ) )
    return results;

  QgsFeatureList featureList;

  // toLayerCoordinates will throw an exception for an 'invalid' point.
  // For example, if you project a world map onto a globe using EPSG 2163
  // and then click somewhere off the globe, an exception will be thrown.
  try
  {
    // create the search rectangle
    double searchRadius = searchRadiusMU();

    QgsRectangle r;
    r.setXMinimum( point.x() - searchRadius );
    r.setXMaximum( point.x() + searchRadius );
    r.setYMinimum( point.y() - searchRadius );
    r.setYMaximum( point.y() + searchRadius );

    r = toLayerCoordinates( layer, r );

    QgsFeatureRequest req;
    req.setFilterRect( r );
    req.setLimit( QSettings().value( "/QField/identify/limit" , 100 ).toInt() );
    req.setFlags( QgsFeatureRequest::ExactIntersect );

    QgsFeatureIterator fit = layer->getFeatures( req );
    QgsFeature f;
    while ( fit.nextFeature( f ) )
      featureList << QgsFeature( f );
  }
  catch ( QgsCsException & cse )
  {
    Q_UNUSED( cse );
    // catch exception for 'invalid' point and proceed with no features found
  }

  bool filter = false;

  QgsRenderContext context( QgsRenderContext::fromMapSettings( mMapSettings->mapSettings() ) );
  context.expressionContext() << QgsExpressionContextUtils::layerScope( layer );
  QgsFeatureRenderer* renderer = layer->renderer();
  if ( renderer && renderer->capabilities() & QgsFeatureRenderer::ScaleDependent )
  {
    // setup scale for scale dependent visibility (rule based)
    renderer->startRender( context, layer->fields() );
    filter = renderer->capabilities() & QgsFeatureRenderer::Filter;
  }

  Q_FOREACH( const QgsFeature& feature, featureList )
  {
    context.expressionContext().setFeature( feature );

    if ( filter && !renderer->willRenderFeature( const_cast<QgsFeature&>( feature ), context ) )
      continue;

    results.append( IdentifyResult( layer, feature ) );
  }

  if ( renderer && renderer->capabilities() & QgsFeatureRenderer::ScaleDependent )
  {
    renderer->stopRender( context );
  }

  return results;
}

/*
MultiFeatureListModel* QgsQuickIdentifyKit::model() const
{
  return mModel;
}

void QgsQuickIdentifyKit::setModel( MultiFeatureListModel* model )
{
  if ( model == mModel )
    return;

  mModel = model;
  emit modelChanged();
}
*/

double QgsQuickIdentifyKit::searchRadiusMU( const QgsRenderContext& context ) const
{
  return mSearchRadiusMm * context.scaleFactor() * context.mapToPixel().mapUnitsPerPixel();
}

double QgsQuickIdentifyKit::searchRadiusMU() const
{
  QgsRenderContext context = QgsRenderContext::fromMapSettings( mMapSettings->mapSettings() );
  return searchRadiusMU( context );
}

QgsRectangle QgsQuickIdentifyKit::toLayerCoordinates( QgsMapLayer* layer, const QgsRectangle& rect ) const
{
  return mMapSettings->mapSettings().mapToLayerCoordinates( layer, rect );
}

double QgsQuickIdentifyKit::searchRadiusMm() const
{
  return mSearchRadiusMm;
}

void QgsQuickIdentifyKit::setSearchRadiusMm( double searchRadiusMm )
{
  if ( mSearchRadiusMm == searchRadiusMm )
    return;

  mSearchRadiusMm = searchRadiusMm;
  emit searchRadiusMmChanged();
}
