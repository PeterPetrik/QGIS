/***************************************************************************
  qgsquickidentifykit.h
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

#ifndef QGSQUICKIDENTIFYKIT_H
#define QGSQUICKIDENTIFYKIT_H

#include <QObject>
#include <QPair>

#include "qgis_quick.h"
#include <qgsfeature.h>
#include <qgspoint.h>
#include <qgsmapsettings.h>
#include <qgsrendercontext.h>
#include <qgsquickidentifyresult.h>

class QgsQuickProject;
class QgsMapLayer;
class QgsQuickMapSettings;
class QgsVectorLayer;

/**
 * \ingroup quick
 * Convinient set of tools to get a list of QgsFeatures in a defined radius from a point.
 * Also possible to get a feature with the closest distance to the point or feature(s) from
 * specified QgsVectorLayer
 *
 * \note QML Type: IdentifyKit
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickIdentifyKit : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QgsQuickMapSettings* mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( double searchRadiusMm READ searchRadiusMm WRITE setSearchRadiusMm NOTIFY searchRadiusMmChanged )

  public:   
    explicit QgsQuickIdentifyKit( QObject *parent = 0 );

    QgsQuickMapSettings* mapSettings() const;
    void setMapSettings( QgsQuickMapSettings* mapSettings );

    double searchRadiusMm() const;
    void setSearchRadiusMm( double searchRadiusMm );

    Q_INVOKABLE QgsFeature identifyOne( QgsVectorLayer* layer, const QPointF& point );
    Q_INVOKABLE QgsQuickIdentifyResult identifyOne( const QPointF& point );

    Q_INVOKABLE QList<QgsQuickIdentifyResult> identify( const QPointF& point );
    Q_INVOKABLE QgsFeatureList identify( QgsVectorLayer* layer, const QPointF& point );

   signals:
    void mapSettingsChanged();
    void searchRadiusMmChanged();

  private:
    QgsQuickProject* mProject;
    QgsQuickMapSettings* mMapSettings;

    double searchRadiusMU( const QgsRenderContext& context ) const;
    double searchRadiusMU() const;

    QgsRectangle toLayerCoordinates( QgsMapLayer* layer, const QgsRectangle& rect ) const;
    QgsFeatureList identifyVectorLayer( QgsVectorLayer* layer, const QgsPointXY& point ) const;

    double mSearchRadiusMm;
};

#endif // QGSQUICKIDENTIFYKIT_H
