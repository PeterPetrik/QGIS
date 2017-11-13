/***************************************************************************
                            androidplatformutilities.h  -  utilities for qfield

                              -------------------
              begin                : February 2016
              copyright            : (C) 2016 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKUTILS_H
#define QGSQUICKUTILS_H

#include "qgis_quick.h"

#include <QObject>
#include <QString>

class PictureSource;

class QUICK_EXPORT QgsQuickUtils: public QObject
{
  Q_OBJECT
  public:
    static QgsQuickUtils* instance();

    virtual PictureSource* getPicture( const QString &prefix );
    virtual void open( const QString& data, const QString& type );

  protected:
    explicit QgsQuickUtils(QObject *parent = 0);
    static QgsQuickUtils* sInstance;
};

#endif // QGSQUICKUTILS_H
