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
#include "qgsquickproject.h"
#include "qgsquickmapsettings.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickidentifykit.h"

#include <qgsmaplayer.h>
#include <qgsquickscalebarkit.h>

#include <qqml.h>

void QgisQuickPlugin::registerTypes(const char *uri)
{
  qDebug("REGISTERING QQmlExtensionInterface: QgisQuickPlugin");

  qmlRegisterType<QgsQuickMapSettings>(uri, 1, 0, "MapSettings");
  qmlRegisterType<QgsQuickMapCanvasMap>(uri, 1, 0, "MapCanvasMap");
  qmlRegisterType<QgsQuickProject>(uri, 1, 0, "Project");
  qmlRegisterType<QgsQuickScaleBarKit>(uri, 1, 0, "ScaleBarKit");
  qmlRegisterType<QgsQuickIdentifyKit>(uri, 1, 0, "IdentifyKit");

  qRegisterMetaType< QList<QgsMapLayer*> >( "QList<QgsMapLayer*>" );

}

