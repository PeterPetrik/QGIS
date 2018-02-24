/***************************************************************************
  main.cpp
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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtDebug>
#include <QQmlError>
#include <QDesktopWidget>
#include <QQmlContext>
#include <qqml.h>

#include <QCommandLineParser>
#include <qgis.h>

#include "qgsapplication.h"
#include "qgsproject.h"
#include "qgsmaplayer.h"
#include "qgsvectorlayer.h"
#include "qgslayertreegroup.h"
#include "qgslayertree.h"
#include <qgsmessagelog.h>
#include "qgsquickutils.h"
#include "qgsquicklayertreemodel.h"

float calculateDevicePixels()
{
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int dpi = dpiX < dpiY ? dpiX : dpiY; // In case of asymetrical DPI. Improbable
  float dp = dpi * 0.00768443;
  return dp;
}

int loadTestLayers(QList<QString> layersName, QgsProject* project)
{

#ifdef QGIS_QUICK_DATA_PATH
    QString dataDir( QGIS_QUICK_DATA_PATH );
#else
    QString dataDir( ::getenv( "QGIS_QUICK_DATA_PATH" ) );
#endif
    //QList<QgsMapLayer *> layers;
    foreach ( QString fileName, layersName )
    {
        QFileInfo fileInfo( dataDir + "/" + fileName);
        //QgsVectorLayer *layer = new QgsVectorLayer( fileInfo.filePath(),fileInfo.completeBaseName(), "ogr" );
        QgsVectorLayer *layer = new QgsVectorLayer( fileInfo.filePath(), fileName);
        project->addMapLayer( layer );
    }

    return 0;
}

int main( int argc, char *argv[] )
{
  // 1) Initialize QGIS
  QgsApplication app( argc, argv, true );

  QgsApplication::init();
  QgsApplication::initQgis();

  QQmlEngine engine;
  qmlRegisterType< QgsQuickLayerTreeModel >("QgisQuickApp", 1, 0, "LayerTreeModel");

  // 2) Load QGIS Project
 #ifdef QGIS_QUICK_DATA_PATH
   QString dataDir( QGIS_QUICK_DATA_PATH );
 #else
   QString dataDir( ::getenv( "QGIS_QUICK_DATA_PATH" ) );
 #endif
   QString projectFile = dataDir + "/test_project.qgs";
   qDebug() << "project file: " << projectFile;
   QgsProject project;
   bool res = project.read( projectFile );
   Q_ASSERT( res );

   engine.rootContext()->setContextProperty( "__project", &project );


  // 3) Load project's layers
  QgsLayerTreeGroup *root = project.layerTreeRoot();
  QList<QgsMapLayer *> layers;
  qDebug() << project.count() << " layers loading...";
  foreach ( QgsLayerTreeLayer *nodeLayer, root->findLayers() )
  {
    if ( nodeLayer->isVisible() )
    {
      QgsMapLayer *layer = nodeLayer->layer();
      if ( layer->isValid() )
      {
        qDebug() << "Found layer: " << layer->name();
      }
    }
  }
  engine.rootContext()->setContextProperty( "__layers", QVariant::fromValue( layers ) );

  // Set Device Pixels
  float dp = calculateDevicePixels();
  QgsQuickUtils::instance()->setProperty( "dp", dp );

  // Set simulated position for desktop builds
  bool use_simulated_position = true;
  engine.rootContext()->setContextProperty( "__use_simulated_position", use_simulated_position );

  QQmlComponent component(&engine,QUrl(QStringLiteral("qrc:/main.qml")));
  QObject *object = component.create();
  \
  if ( !component.errors().isEmpty() )
  {
    qDebug( "%s", QgsApplication::showSettings().toLocal8Bit().data() );

    qDebug() << "****************************************";
    qDebug() << "*****        QML errors:           *****";
    qDebug() << "****************************************";
    for ( const QQmlError &error : component.errors() )
    {
      qDebug() << "  " << error;
    }
    qDebug() << "****************************************";
    qDebug() << "****************************************";
  }

  if ( object == 0 )
  {
    qDebug() << "FATAL ERROR: unable to create main.qml";
    return EXIT_FAILURE;
  }



  // Set up the QSettings environment must be done after qapp is created
  QCoreApplication::setOrganizationName( "QGIS" );
  QCoreApplication::setOrganizationDomain( "qgis.org" );
  QCoreApplication::setApplicationName( "QgsQuick Test App" );
  QCoreApplication::setApplicationVersion( Qgis::QGIS_VERSION );

  QCommandLineParser parser;
  parser.addVersionOption();
  parser.process( app );

  // Add some data for debugging if needed (visible in the final customer app)
  QRect rec = QApplication::desktop()->screenGeometry();
  int height = rec.height();
  int width = rec.width();
  QgsApplication::messageLog()->logMessage( "screen: " + QString::number( width ) + "x" + QString::number( height ) );
  QgsApplication::messageLog()->logMessage( "device pixels: " + QString::number( dp ) );
  QgsApplication::messageLog()->logMessage( "data directory: " + dataDir );
  QgsApplication::messageLog()->logMessage( "All up and running" );


  qDebug() << " loading finishing..." << QString::number( dp ) ;
  return app.exec();
}

