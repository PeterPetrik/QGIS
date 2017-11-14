/***************************************************************************
 qgsquickpicturesource.h
  --------------------------------------
  Date                 : 5.7.2016
  Copyright            : (C) 2016 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKPICTURESOURCE_H
#define QGSQUICKPICTURESOURCE_H

#include "qgis_quick.h"

#include <QObject>
#include <QString>

#ifdef ANDROID
#include <QAndroidActivityResultReceiver>
#endif

/**
 * A picture source object will be created when a new picture is requested from the system.
 * It should be subclassed and filled with platform specific code.
 */
class QUICK_EXPORT QgsQuickPictureSource :
        public QObject
#ifdef ANDROID
        , public QAndroidActivityResultReceiver
#endif
{
    Q_OBJECT

  public:
    explicit QgsQuickPictureSource( const QString& prefix = QString() );
    virtual ~QgsQuickPictureSource();

#ifdef ANDROID
    void handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject& data ) override;
#endif

  signals:
    /**
     * Emit this signal when a picture really has been received.
     */
    void pictureReceived( const QString& path );

#ifdef ANDROID
  private:
    QString mPrefix;
#endif
};

#endif // QGSQUICKPICTURESOURCE_H
