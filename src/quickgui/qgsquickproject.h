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

class QgsQuickProject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString projectFile READ projectFile WRITE setProjectFile NOTIFY projectFileChanged)
  Q_PROPERTY(QList< QgsMapLayer* > layers READ layers NOTIFY projectFileChanged)
  Q_PROPERTY(QgsQuickLayerTreeModel* layerTreeModel READ layerTreeModel NOTIFY layerTreeModelChanged)

public:
  static QgsQuickProject* instance();

  void setProjectFile(const QString& filename);
  QString projectFile() const { return mFilename; }

  QgsQuickLayerTreeModel* layerTreeModel() const;

  QList< QgsMapLayer* > layers() const;

signals:
  void projectFileChanged();
  void layerTreeModelChanged();

public slots:

protected:
  explicit QgsQuickProject(QObject *parent = 0);

  static QgsQuickProject* sInstance;

  QgsQuickLayerTreeModel* mLayerTreeModel;
  QString mFilename;
};

#endif // QGSQUICKPROJECT_H
