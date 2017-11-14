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

#include <qgsproject.h>
#include <qgslayertree.h>

QgsQuickProject::QgsQuickProject(QObject *parent)
  : QObject(parent)
{
    mProject = new QgsProject(parent);
    connect( mProject, &QgsProject::homePathChanged, this, &QgsQuickProject::homePathChanged );
}

QgsProject* QgsQuickProject::project() const
{
    return mProject;
}

QgsQuickProject::~QgsQuickProject()
{
    delete mProject;
    mProject = 0;
}

QString QgsQuickProject::projectFile() const {
    return mProject->fileName();
}

QString QgsQuickProject::homePath() const {
    return mProject->homePath();
}

QgsRelationManager* QgsQuickProject::relationManager() const {
    return mProject->relationManager();
}

void QgsQuickProject::setProjectFile(const QString& filename)
{
  mProject->setFileName(filename);
  bool res = mProject->read();
  qDebug("load project: %d", res);
  emit projectFileChanged();
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
