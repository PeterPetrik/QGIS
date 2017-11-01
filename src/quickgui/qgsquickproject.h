/***************************************************************************
  qgsquickplugin.h
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

#ifndef QGSQUICKPROJECT_H
#define QGSQUICKPROJECT_H

#include <QObject>

class QgsMapLayer;
class QgsQuickLayerTreeModel;
class QgsProject;

class QgsQuickProject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString fileName READ projectFile WRITE setProjectFile NOTIFY projectFileChanged)
  Q_PROPERTY(QList< QgsMapLayer* > layers READ layers NOTIFY projectFileChanged)
  Q_PROPERTY(QgsQuickLayerTreeModel* layerTreeModel READ layerTreeModel NOTIFY projectFileChanged)

public:
  void setProjectFile(const QString& filename);
  QString projectFile() const;

  QgsQuickLayerTreeModel* layerTreeModel() const;
  QList< QgsMapLayer* > layers() const;

signals:
  void projectFileChanged();

public slots:

protected:
  explicit QgsQuickProject(QObject *parent = 0);
  ~QgsQuickProject();

private:
  QgsProject* mProject;
  QgsQuickLayerTreeModel* mLayerTreeModel;
};

#endif // QGSQUICKPROJECT_H
