#include "qgsquickplugin.h"
#include "qgsquickproject.h"

#include "mapengine.h"
#include "mapimage.h"
#include "maplayer.h"
#include "mapview.h"


#include "qgsnetworkaccessmanager.h"
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgsmaplayer.h>

#include <qqml.h>

static QObject *_projectProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED(engine)
  Q_UNUSED(scriptEngine)
  return QgsQuickProject::instance();
}


void QgisQuickPlugin::registerTypes(const char *uri)
{
  qDebug("REGISTERING QQmlExtensionInterface: QgisQuickPlugin");

  qmlRegisterType<MapEngine>(uri, 1, 0, "MapEngine");
  qmlRegisterType<MapView>(uri, 1, 0, "MapView");
  qmlRegisterType<MapImage>(uri, 1, 0, "MapImage");
  qmlRegisterType<MapLayer>(uri, 1, 0, "MapLayer");

  qmlRegisterSingletonType<QgsQuickProject>(uri, 1, 0, "Project", _projectProvider);

  qRegisterMetaType< QList<QgsMapLayer*> >( "QList<QgsMapLayer*>" );
}
