/***************************************************************************
      qgsgeopackageitemguiprovider.h
      -------------------
    begin                : June, 2019
    copyright            : (C) 2019 by Peter Petrik
    email                : zilolv at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsgeopackageitemguiprovider.h"

#include <QAction>
#include <QMenu>
#include <QString>
#include <QMessageBox>

#include "qgsvectorlayer.h"
#include "qgsrasterlayer.h"
#include "qgsgeopackagedataitems.h"
#include "qgsnewnamedialog.h"
#include "qgsnewgeopackagelayerdialog.h"
#include "qgsmessageoutput.h"
#include "qgsapplication.h"
#include "qgsgeopackagerasterwritertask.h"
#include "qgsvectorlayerexporter.h"

void QgsGeoPackageItemGuiProvider::populateContextMenu( QgsDataItem *item, QMenu *menu,
    const QList<QgsDataItem *> &,
    QgsDataItemGuiContext )
{
  if ( QgsGeoPackageAbstractLayerItem *layerItem = qobject_cast< QgsGeoPackageAbstractLayerItem * >( item ) )
  {
    // Messages are different for files and tables
    const QString message = QObject::tr( "Delete Layer “%1”…" ).arg( layerItem->name() );
    QAction *actionDeleteLayer = new QAction( message, menu );

    // IMPORTANT - we need to capture the stuff we need, and then hand the slot
    // off to a static method. This is because it's possible for this item
    // to be deleted in the background on us (e.g. by a parent directory refresh)
    const QString uri = layerItem->uri();
    const QString path = layerItem->path();
    QPointer< QgsDataItem > parentItem( layerItem->parent() );
    connect( actionDeleteLayer, &QAction::triggered, this, [ uri, path, parentItem ]
    {
      //
    } );
    menu->addAction( actionDeleteLayer );
  }

  if ( QgsGeoPackageCollectionItem *collectionItem = qobject_cast< QgsGeoPackageCollectionItem * >( item ) )
  {

  }
}


bool QgsGeoPackageItemGuiProvider::acceptDrop(QgsDataItem* item, QgsDataItemGuiContext )
{
  if ( qobject_cast< QgsGeoPackageCollectionItem * >( item ) )
  {
    return true;
  }
  return false;
}

bool QgsGeoPackageItemGuiProvider::handleDrop(QgsDataItem* item, QgsDataItemGuiContext , const QMimeData* data, Qt::DropAction )
{
  if ( QgsGeoPackageCollectionItem* collectionItem = qobject_cast< QgsGeoPackageCollectionItem * >( item ) )
  {
    return handleDropGeopackage( collectionItem, data );
  }
  return false;
}

bool QgsGeoPackageItemGuiProvider::handleDropGeopackage( QgsGeoPackageCollectionItem* item, const QMimeData *data )
{

  if ( !QgsMimeDataUtils::isUriList( data ) )
    return false;

  QString uri;

  QStringList importResults;
  bool hasError = false;

  // Main task
  std::unique_ptr< QgsConcurrentFileWriterImportTask > mainTask( new QgsConcurrentFileWriterImportTask( tr( "GeoPackage import" ) ) );
  QgsTaskList importTasks;

  const auto lst = QgsMimeDataUtils::decodeUriList( data );
  for ( const QgsMimeDataUtils::Uri &dropUri : lst )
  {
    // Check that we are not copying over self
    if ( dropUri.uri.startsWith( item->path() ) )
    {
      importResults.append( tr( "You cannot import layer %1 over itself!" ).arg( dropUri.name ) );
      hasError = true;

    }
    else
    {
      QgsMapLayer *srcLayer = nullptr;
      bool owner;
      bool isVector = false;
      QString error;
      // Common checks for raster and vector
      // aspatial is treated like vector
      if ( dropUri.layerType == QStringLiteral( "vector" ) )
      {
        // open the source layer
        srcLayer = dropUri.vectorLayer( owner, error );
        isVector = true;
      }
      else if ( dropUri.layerType == QStringLiteral( "mesh" ) )
      {
        // unsupported
        hasError = true;
        continue;
      }
      else
      {
        srcLayer = dropUri.rasterLayer( owner, error );
      }
      if ( !srcLayer )
      {
        importResults.append( tr( "%1: %2" ).arg( dropUri.name, error ) );
        hasError = true;
        continue;
      }

      if ( srcLayer->isValid() )
      {
        uri = item->path();
        QgsDebugMsgLevel( "URI " + uri, 3 );

        // check if the destination layer already exists
        bool exists = false;
        const auto c( item->children() );
        for ( const QgsDataItem *child : c )
        {
          if ( child->name() == dropUri.name )
          {
            exists = true;
          }
        }

        if ( exists && !isVector )
        {
          QMessageBox::warning( nullptr, tr( "Cannot Overwrite Layer" ),
                                tr( "Destination layer <b>%1</b> already exists. Overwriting with raster layers is not currently supported." ).arg( dropUri.name ) );
        }
        else if ( ! exists || QMessageBox::question( nullptr, tr( "Overwrite Layer" ),
                  tr( "Destination layer <b>%1</b> already exists. Do you want to overwrite it?" ).arg( dropUri.name ), QMessageBox::Yes |  QMessageBox::No ) == QMessageBox::Yes )
        {
          if ( isVector ) // Import vectors and aspatial
          {
            QgsVectorLayer *vectorSrcLayer = qobject_cast < QgsVectorLayer * >( srcLayer );
            QVariantMap options;
            options.insert( QStringLiteral( "driverName" ), QStringLiteral( "GPKG" ) );
            options.insert( QStringLiteral( "update" ), true );
            options.insert( QStringLiteral( "overwrite" ), true );
            options.insert( QStringLiteral( "layerName" ), dropUri.name );
            options.insert( QStringLiteral( "forceSinglePartGeometryType" ), true );
            QgsVectorLayerExporterTask *exportTask = new QgsVectorLayerExporterTask( vectorSrcLayer, uri, QStringLiteral( "ogr" ), vectorSrcLayer->crs(), options, owner );
            mainTask->addSubTask( exportTask, importTasks );
            importTasks << exportTask;
            // when export is successful:
            connect( exportTask, &QgsVectorLayerExporterTask::exportComplete, item, [ = ]()
            {
              // this is gross - TODO - find a way to get access to messageBar from data items
              QMessageBox::information( nullptr, tr( "Import to GeoPackage database" ), tr( "Import was successful." ) );
              item->refreshConnections();
            } );

            // when an error occurs:
            connect( exportTask, &QgsVectorLayerExporterTask::errorOccurred, item, [ = ]( int error, const QString & errorMessage )
            {
              if ( error != QgsVectorLayerExporter::ErrUserCanceled )
              {
                QgsMessageOutput *output = QgsMessageOutput::createMessageOutput();
                output->setTitle( tr( "Import to GeoPackage database" ) );
                output->setMessage( tr( "Failed to import some vector layers!\n\n" ) + errorMessage, QgsMessageOutput::MessageText );
                output->showMessage();
              }
            } );

          }
          else  // Import raster
          {
            QgsGeoPackageRasterWriterTask  *exportTask = new QgsGeoPackageRasterWriterTask( dropUri, item->path() );
            mainTask->addSubTask( exportTask, importTasks );
            importTasks << exportTask;
            // when export is successful:
            connect( exportTask, &QgsGeoPackageRasterWriterTask::writeComplete, item, [ = ]()
            {
              // this is gross - TODO - find a way to get access to messageBar from data items
              QMessageBox::information( nullptr, tr( "Import to GeoPackage database" ), tr( "Import was successful." ) );
              item->refreshConnections();
            } );

            // when an error occurs:
            connect( exportTask, &QgsGeoPackageRasterWriterTask::errorOccurred, item, [ = ]( QgsGeoPackageRasterWriter::WriterError error, const QString & errorMessage )
            {
              if ( error != QgsGeoPackageRasterWriter::WriterError::ErrUserCanceled )
              {
                QgsMessageOutput *output = QgsMessageOutput::createMessageOutput();
                output->setTitle( tr( "Import to GeoPackage database" ) );
                output->setMessage( tr( "Failed to import some raster layers!\n\n" ) + errorMessage, QgsMessageOutput::MessageText );
                output->showMessage();
              }
              // Always try to delete the imported raster, in case the gpkg has been left
              // in an inconsistent status. Ignore delete errors.
              QString deleteErr;
              item->deleteGeoPackageRasterLayer( QStringLiteral( "GPKG:%1:%2" ).arg( item->path(), dropUri.name ), deleteErr );
            } );

          }
        } // do not overwrite
      }
      else
      {
        importResults.append( tr( "%1: Not a valid layer!" ).arg( dropUri.name ) );
        hasError = true;
      }
    } // check for self copy
  } // for each

  if ( hasError )
  {
    QgsMessageOutput *output = QgsMessageOutput::createMessageOutput();
    output->setTitle( tr( "Import to GeoPackage database" ) );
    output->setMessage( tr( "Failed to import some layers!\n\n" ) + importResults.join( QStringLiteral( "\n" ) ), QgsMessageOutput::MessageText );
    output->showMessage();
  }
  if ( ! importTasks.isEmpty() )
  {
    QgsApplication::taskManager()->addTask( mainTask.release() );
  }
  return true;
}
