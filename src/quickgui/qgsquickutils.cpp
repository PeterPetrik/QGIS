/***************************************************************************
  qgsquickutils.cpp
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

#include "qgsquickutils.h"
#include "qgsquickpicturesource.h"

#include <QMap>
#include <QString>
#include <QDebug>

#ifdef ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#endif

QgsQuickUtils* QgsQuickUtils::sInstance = 0;

QgsQuickUtils* QgsQuickUtils::instance()
{
  if (!sInstance)
    sInstance = new QgsQuickUtils();
  return sInstance;
}

QgsQuickUtils::QgsQuickUtils(QObject *parent):QObject(parent)
{
}

PictureSource* QgsQuickUtils::getPicture( const QString& prefix )
{
#ifdef ANDROID
  QAndroidJniObject actionImageCapture = QAndroidJniObject::getStaticObjectField( "android/provider/MediaStore", "ACTION_IMAGE_CAPTURE", "Ljava/lang/String;" );

  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionImageCapture.object<jstring>() );

  PictureSource* pictureSource = nullptr;

  if ( actionImageCapture.isValid() && intent.isValid() )
  {
    pictureSource = new PictureSource( prefix );
    QtAndroid::startActivity( intent.object<jobject>(), 101, pictureSource );
  }
  else
  {
    qDebug() << "Something went wrong creating the picture request";
  }

  return pictureSource;
#else
    Q_UNUSED(prefix);
#endif
}

void QgsQuickUtils::open( const QString& data, const QString& type )
{
#ifdef ANDROID
  QAndroidJniObject actionView = QAndroidJniObject::getStaticObjectField( "android/intent/action", "ACTION_VIEW", "Ljava/lang/String;" );

  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionView.object<jstring>() );

  QAndroidJniObject jDataString = QAndroidJniObject::fromString( data );
  QAndroidJniObject jType = QAndroidJniObject::fromString( type );

  QAndroidJniObject jData = QAndroidJniObject::callStaticObjectMethod( "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", jDataString.object<jstring>() );

  intent.callObjectMethod( "setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;", jData.object<jobject>(), jType.object<jstring>() );

  QtAndroid::startActivity( intent.object<jobject>(), 102 );
#else
    Q_UNUSED(data);
    Q_UNUSED(type);
#endif
}
