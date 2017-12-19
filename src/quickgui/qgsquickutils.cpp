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
#include "qgsquickstyle.h"
#include "qgscoordinatereferencesystem.h"
#include "qgscoordinatetransform.h"
#include "qgsvectorlayer.h"

#include <QMap>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QFileInfo>
#include <QLibraryInfo>

#ifdef ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

QgsQuickUtils* QgsQuickUtils::sInstance = 0;

QgsQuickUtils* QgsQuickUtils::instance()
{
  if (!sInstance)
  {
    qDebug() << "QgsQuickUtils created: " << QThread::currentThreadId();
    sInstance = new QgsQuickUtils();
  }
  return sInstance;
}

QgsQuickUtils::QgsQuickUtils(QObject *parent):
    QObject(parent)
{
    mCoordinateReferenceSystem = new QgsCoordinateReferenceSystem();
}

QgsQuickUtils::~QgsQuickUtils() {

    if (mCoordinateReferenceSystem) {
        delete mCoordinateReferenceSystem;
        mCoordinateReferenceSystem = 0;
    }
}

QgsCoordinateReferenceSystem QgsQuickUtils::coordinateReferenceSystemFromEpsgId(long epsg) const {
    return QgsCoordinateReferenceSystem::fromEpsgId(epsg);
}

void QgsQuickUtils::setStyle( QgsQuickStyle *style ) {
  Q_ASSERT( !mStyle && "Style must be assigned only once!" );
  mStyle = style;
}

QgsQuickStyle* QgsQuickUtils::style() const {
    return mStyle;
}

QgsPointXY QgsQuickUtils::pointXYFactory(double x, double y) const {
    return QgsPointXY(x, y);
}

QgsPoint QgsQuickUtils::pointFactory(double x, double y) const {
    return QgsPoint(x, y);
}


QgsPointXY QgsQuickUtils::transformPoint(QgsCoordinateReferenceSystem srcCrs, QgsCoordinateReferenceSystem destCrs, QgsPointXY srcPoint) const {
    QgsCoordinateTransform mTransform;
    mTransform.setSourceCrs(srcCrs);
    mTransform.setDestinationCrs(destCrs);
    mTransform.initialize();
    QgsPointXY pt = mTransform.transform( srcPoint );
    return pt;
}

QgsFeature QgsQuickUtils::featureFromGps(const QgsPoint &gpsPoint, QgsVectorLayer *layer) const
{
  QgsGeometry geom( gpsPoint.clone() );
  geom.transform( QgsCoordinateTransform( QgsCoordinateReferenceSystem( "EPSG:4326" ), layer->crs() ) );

  QgsFeature f;
  f.setGeometry( geom );
  f.setFields( layer->fields() );
  QgsAttributes attrs( f.fields().count() );
  f.setAttributes( attrs );
  return f;
}

bool QgsQuickUtils::fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

QUrl QgsQuickUtils::getThemeIcon(const QString& name) {
    Q_ASSERT(mStyle);

    float ppi = mStyle->devicePixels() / 0.00768443;
    QString ppitype;

    if ( ppi >= 360 )
        ppitype = "xxxhdpi";
    else if ( ppi >= 270 )
        ppitype = "xxhdpi";
    else if ( ppi >= 180 )
        ppitype = "xhdpi";
    else if ( ppi >= 135 )
        ppitype = "hdpi";
    else
        ppitype = "mdpi";

    // Check in custom dir
    QString path(mStyle->themeDir() + "/" + ppitype + "/" + name + ".png");
    if (!fileExists(path)) {
        path = "qrc:/" + ppitype + "/" + name + ".png";
    }
    qDebug() << "Using icon " << name << " from " << path;
    return QUrl(path);
}

QgsQuickPictureSource* QgsQuickUtils::getPicture( const QString& prefix )
{
#ifdef ANDROID
  QAndroidJniObject actionImageCapture = QAndroidJniObject::getStaticObjectField( "android/provider/MediaStore", "ACTION_IMAGE_CAPTURE", "Ljava/lang/String;" );

  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionImageCapture.object<jstring>() );

  QgsQuickPictureSource* pictureSource = nullptr;

  if ( actionImageCapture.isValid() && intent.isValid() )
  {
    pictureSource = new QgsQuickPictureSource( prefix );
    QtAndroid::startActivity( intent.object<jobject>(), 101, pictureSource );
  }
  else
  {
    qDebug() << "Something went wrong creating the picture request";
  }

  return pictureSource;
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
