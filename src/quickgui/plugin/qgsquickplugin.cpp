/***************************************************************************
  qgsquickplugin.cpp
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

#include "qgsquickplugin.h"
#include "qgsquickmapsettings.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickidentifykit.h"
#include "qgsquickattributeformmodel.h"
#include "qgsquickattributeformmodelbase.h"
#include "qgsquickfeaturemodel.h"
#include "qgsquickgeometry.h"
#include "qgsquickrubberband.h"
#include "qgsquickrubberbandmodel.h"
#include "qgsquicksgrubberband.h"
#include "qgsquicksubmodel.h"
#include "qgsquickpicturesource.h"

#include <qgsmaplayer.h>
#include <qgsquickscalebarkit.h>
#include <qgsquickutils.h>
#include <qgsrelationmanager.h>
#include "qgsfeature.h"
#include <qgsproject.h>

#include <qgsvectorlayer.h>
#include "qgsquickidentifyresult.h"
#include "qgsquickstyle.h"

#include <qqml.h>

static QObject *_utilsProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return QgsQuickUtils::instance();
}

static QObject *_styleProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return QgsQuickUtils::instance()->style();
}

void QgisQuickPlugin::registerTypes(const char *uri)
{
  qDebug("REGISTERING QQmlExtensionInterface: QgisQuick");

  qmlRegisterType<QgsQuickMapSettings>(uri, 1, 0, "MapSettings");
  qmlRegisterType<QgsQuickMapCanvasMap>(uri, 1, 0, "MapCanvasMap");
  qmlRegisterType<QgsQuickScaleBarKit>(uri, 1, 0, "ScaleBarKit");
  qmlRegisterType<QgsQuickIdentifyKit>(uri, 1, 0, "IdentifyKit");

  qmlRegisterType<QgsQuickFeatureModel>(uri, 1, 0, "FeatureModel");
  qmlRegisterType<QgsQuickGeometry>(uri, 1, 0, "Geometry");
  qmlRegisterType<QgsQuickRubberband>(uri, 1, 0, "Rubberband");
  qmlRegisterType<QgsQuickRubberbandModel>(uri, 1, 0, "RubberbandModel");
  qmlRegisterType<QgsQuickSubModel>(uri, 1, 0, "SubModel");
  qmlRegisterType<QgsQuickPictureSource>(uri, 1, 0, "PictureSource");
  //qmlRegisterType<QgsQuickIdentifyResult>(uri, 1, 0, "IdentifyResult");

  //qmlRegisterType< QgsFeature >(uri, 1, 0, "Feature" );
  qmlRegisterType<QgsRelationManager>(uri, 1, 0, "RelationManager"); //TODO create separate quick class????
  qmlRegisterType<QgsVectorLayer>(uri, 1, 0, "VectorLayer"); //TODO create separate quick class????
  qmlRegisterType<QgsProject>(uri, 1, 0, "Project");

  qRegisterMetaType<QgsQuickIdentifyResult>("QgsQuickIdentifyResult");
  qRegisterMetaType<QgsPoint>("QgsPoint");
  qRegisterMetaType< QList<QgsMapLayer*> >( "QList<QgsMapLayer*>" );

  //qRegisterMetaType< QList<QgsQuickIdentifyKit::IdentifyResult> >( "QList<IdentifyResult>" );
  //qRegisterMetaType< QgsFeatureList > ( "FeatureList" );
  qRegisterMetaType< QgsFeature > ( "QgsFeature ");
  qRegisterMetaType< QgsFeatureId > ( "QgsFeatureId ");
  qRegisterMetaType< QgsAttributes > ( "QgsAttributes ");

  qmlRegisterType<QgsQuickAttributeFormModel>(uri, 1, 0, "AttributeFormModel");
  //qmlRegisterType<QgsQuickAttributeFormModelBase>(uri, 1, 0, "AttributeFormModelBase");
  //qRegisterMetaType<QgsQuickAttributeFormModel>( "AttributeFormModel");
  //qRegisterMetaType<QgsQuickAttributeFormModelBase>( "AttributeFormModelBase");

  qmlRegisterSingletonType<QgsQuickUtils>(uri, 1, 0, "Utils", _utilsProvider);
  qmlRegisterSingletonType<QgsQuickStyle>(uri, 1, 0, "Style", _styleProvider);

  qDebug("REGISTERING FINISHED");
}

