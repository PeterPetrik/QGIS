#ifndef QGSQUICKPLUGIN_H
#define QGSQUICKPLUGIN_H

#include <QQmlExtensionPlugin>

class QgisQuickPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
  void registerTypes(const char *uri);
};

#endif // QGSQUICKPLUGIN_H

