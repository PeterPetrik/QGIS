#include "plugin.h"
#include "mapengine.h"
#include "mapimage.h"
#include "maplayer.h"
#include "mapview.h"
#include "project.h"
#include "qgsnetworkaccessmanager.h"

#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgsmaplayer.h>

#include <qqml.h>

static QObject *_projectProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return Project::instance();
}


void Qgis_Mobile_ComponentsPlugin::registerTypes(const char *uri)
{
  qDebug("REGISTERING QGIS plugin");

  // @uri qgis
  qmlRegisterType<MapEngine>(uri, 1, 0, "MapEngine");
  qmlRegisterType<MapView>(uri, 1, 0, "MapView");
  qmlRegisterType<MapImage>(uri, 1, 0, "MapImage");
  qmlRegisterType<MapLayer>(uri, 1, 0, "MapLayer");

  qmlRegisterSingletonType<Project>(uri, 1, 0, "Project", _projectProvider);

  qRegisterMetaType< QList<QgsMapLayer*> >( "QList<QgsMapLayer*>" );
}
