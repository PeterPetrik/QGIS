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

#include "qgis_quick.h"

#include <QObject>

class QgsMapLayer;
class QgsProject;
class QgsRelationManager;

/* TODO
 * Probably can be removed altogether. We just need a property for layers for mapsettings, everything else
 * can be taken from QgsProject
 */

class QUICK_EXPORT QgsQuickProject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString fileName READ projectFile WRITE setProjectFile NOTIFY projectFileChanged)
  Q_PROPERTY(QList< QgsMapLayer* > layers READ layers NOTIFY projectFileChanged)
  Q_PROPERTY(QgsProject* project READ project)
  Q_PROPERTY(QString homePath READ homePath NOTIFY homePathChanged )
  Q_PROPERTY(QgsRelationManager* relationManager READ relationManager )
  
public:
  void setProjectFile(const QString& filename);
  QString projectFile() const;
  QList< QgsMapLayer* > layers() const;
  QgsProject* project() const; // never returns NULL
  QgsRelationManager* relationManager() const;
  QString homePath() const;

signals:
  void projectFileChanged();
  void homePathChanged();

public slots:

protected:
  explicit QgsQuickProject(QObject *parent = 0);
  ~QgsQuickProject();

private:
  QgsProject* mProject;
};

#endif // QGSQUICKPROJECT_H
