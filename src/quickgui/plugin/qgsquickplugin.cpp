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
#include "qgsquickfeaturemodelhighlight.h"
#include "qgsquickgeometry.h"
#include "qgsquickmaptransform.h"
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
#include "qgsquickcoordinatetransformer.h"
#include "qgsquickpositionkit.h"
#include "qgspointxy.h"
#include "qgsquickmessagelogmodel.h"

#include <qqml.h>

static QObject *_utilsProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return QgsQuickUtils::instance();  // the object will be owned by QML engine and destroyed by the engine on exit
}

static QObject *_styleProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return QgsQuickUtils::instance()->style();  // the object will be owned by QML engine and destroyed by the engine on exit
}

void QgisQuickPlugin::registerTypes(const char *uri)
{
  qDebug("REGISTERING QQmlExtensionInterface: QgisQuick");

  qRegisterMetaType< QList<QgsMapLayer*> >( "QList<QgsMapLayer*>" );
  qRegisterMetaType< QgsAttributes > ( "QgsAttributes ");
  qRegisterMetaType< QgsCoordinateReferenceSystem >("QgsCoordinateReferenceSystem ");
  qRegisterMetaType< QgsFeature > ( "QgsFeature ");
  qRegisterMetaType< QgsFeatureId > ( "QgsFeatureId ");
  qRegisterMetaType< QgsPoint >("QgsPoint");
  qRegisterMetaType< QgsPointXY >("QgsPointXY");
  qRegisterMetaType< QgsQuickIdentifyResult >("QgsQuickIdentifyResult");

  qmlRegisterType< QgsProject >(uri, 0, 1, "Project");
  qmlRegisterType< QgsQuickAttributeFormModel >(uri, 0, 1, "AttributeFormModel");
  qmlRegisterType< QgsQuickCoordinateTransformer >(uri, 0, 1, "CoordinateTransformer");
  qmlRegisterType< QgsQuickFeatureModel >(uri, 0, 1, "FeatureModel");
  qmlRegisterType< QgsQuickFeatureModelHighlight >(uri, 0, 1, "FeatureModelHighlight");
  qmlRegisterType< QgsQuickIdentifyKit >(uri, 0, 1, "IdentifyKit");
  qmlRegisterType< QgsQuickMapCanvasMap >(uri, 0, 1, "MapCanvasMap");
  qmlRegisterType< QgsQuickMapSettings >(uri, 0, 1, "MapSettings");
  qmlRegisterType< QgsQuickMapTransform >(uri, 0, 1, "MapTransform");
  qmlRegisterType< QgsQuickMessageLogModel >(uri, 0, 1, "MessageLogModel");
  qmlRegisterType< QgsQuickPositionKit >(uri, 0, 1, "PositionKit");
  qmlRegisterType< QgsQuickRubberband >(uri, 0, 1, "Rubberband");
  qmlRegisterType< QgsQuickRubberbandModel >(uri, 0, 1, "RubberbandModel");
  qmlRegisterType< QgsQuickScaleBarKit >(uri, 0, 1, "ScaleBarKit");
  qmlRegisterType< QgsQuickSubModel >(uri, 0, 1, "SubModel");
  qmlRegisterType< QgsRelationManager >(uri, 0, 1, "RelationManager"); //TODO create separate quick class????
  qmlRegisterType< QgsVectorLayer >(uri, 0, 1, "VectorLayer"); //TODO create separate quick class????

  qmlRegisterSingletonType< QgsQuickUtils >(uri, 0, 1, "Utils", _utilsProvider);
  qmlRegisterSingletonType< QgsQuickStyle >(uri, 0, 1, "Style", _styleProvider);

  qDebug("REGISTERING FINISHED");
}

