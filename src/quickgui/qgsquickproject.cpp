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

#include "qgsquickproject.h"
#include "qgsquicklayertreemodel.h"

#include <qgsproject.h>
#include <qgslayertree.h>

QgsQuickProject::QgsQuickProject(QObject *parent)
  : QObject(parent)
{
    mProject = new QgsProject(parent);
    mLayerTreeModel = new QgsQuickLayerTreeModel(mProject->layerTreeRoot());
}

QgsQuickProject::~QgsQuickProject()
{
    if (mLayerTreeModel) {
        delete mLayerTreeModel;
        mLayerTreeModel = 0;
    }

    delete mProject;
    mProject = 0;
}

QString QgsQuickProject::projectFile() const {
    return mProject->fileName();
}

void QgsQuickProject::setProjectFile(const QString& filename)
{
  mProject->setFileName(filename);
  bool res = mProject->read();
  qDebug("load project: %d", res);

  if (mLayerTreeModel) {
      delete mLayerTreeModel;
      mLayerTreeModel = 0;
  }
  mLayerTreeModel = new QgsQuickLayerTreeModel(mProject->layerTreeRoot());

  emit projectFileChanged();
}

QgsQuickLayerTreeModel *QgsQuickProject::layerTreeModel() const {
    return mLayerTreeModel;
}

QList< QgsMapLayer* > QgsQuickProject::layers() const
{
  QgsLayerTreeGroup* root = mProject->layerTreeRoot();
  QList< QgsMapLayer* > list;
  foreach (QgsLayerTreeLayer* nodeLayer, root->findLayers())
  {
    if (nodeLayer->isVisible())
      list << nodeLayer->layer();
  }

  return list;
}
