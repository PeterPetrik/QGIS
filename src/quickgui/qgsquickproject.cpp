#include "qgsquickproject.h"

#include <qgsproject.h>
#include <qgslayertree.h>

QgsQuickProject* QgsQuickProject::sInstance = 0;

QgsQuickProject::QgsQuickProject(QObject *parent)
  : QObject(parent)
{

}


QgsQuickProject* QgsQuickProject::instance()
{
  if (!sInstance)
    sInstance = new QgsQuickProject();
  return sInstance;
}

void QgsQuickProject::setProjectFile(const QString& filename)
{
  mFilename = filename;

  QgsProject::instance()->setFileName(filename);
  bool res = QgsProject::instance()->read();
  qDebug("load project: %d", res);

  emit projectFileChanged();
}

QList< QgsMapLayer* > QgsQuickProject::layers() const
{
  QgsLayerTreeGroup* root = QgsProject::instance()->layerTreeRoot();
  //qDebug("root: %s", root->dump().toLocal8Bit().data());
  QList< QgsMapLayer* > list;
  foreach (QgsLayerTreeLayer* nodeLayer, root->findLayers())
  {
    if (nodeLayer->isVisible())
      list << nodeLayer->layer();
  }

  return list;
}
