/***************************************************************************
  qgsmeshlayerproperties.cpp
  --------------------------
    begin                : Jun 2018
    copyright            : (C) 2018 by Peter Petrik
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

#include <limits>
#include <typeinfo>

#include "qgisapp.h"
#include "qgsapplication.h"
#include "qgscontrastenhancement.h"
#include "qgscoordinatetransform.h"
#include "qgscubicrasterresampler.h"
#include "qgsprojectionselectiondialog.h"
#include "qgslogger.h"
#include "qgsmapcanvas.h"
#include "qgsmaplayerstyleguiutils.h"
#include "qgsmaptoolemitpoint.h"
#include "qgsmaptopixel.h"
#include "qgsmetadatawidget.h"
#include "qgsproject.h"
#include "qgsmeshlayer.h"
#include "qgsmeshlayerproperties.h"
#include "qgssettings.h"
#include "qgsrenderermeshpropertieswidget.h"

#include <QTableWidgetItem>
#include <QHeaderView>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>
#include <QPolygonF>
#include <QColorDialog>
#include <QList>
#include <QMouseEvent>
#include <QVector>


QgsMeshLayerProperties::QgsMeshLayerProperties( QgsMapLayer *lyr, QgsMapCanvas *canvas, QWidget *parent, Qt::WindowFlags fl )
  : QgsOptionsDialogBase( QStringLiteral( "MeshLayerProperties" ), parent, fl )
  , mMeshLayer( qobject_cast<QgsMeshLayer *>( lyr ) )
{
  setupUi( this );
  mRendererMeshPropertiesWidget = new QgsRendererMeshPropertiesWidget( lyr, canvas, this );
  mOptsPage_StyleContent->layout()->addWidget( mRendererMeshPropertiesWidget );


  connect( mLayerOrigNameLineEd, &QLineEdit::textEdited, this, &QgsMeshLayerProperties::mLayerOrigNameLineEd_textEdited );
  connect( mCrsSelector, &QgsProjectionSelectionWidget::crsChanged, this, &QgsMeshLayerProperties::mCrsSelector_crsChanged );
  connect( mAddDatasetButton, &QPushButton::clicked, this, &QgsMeshLayerProperties::mAddDatasetButton_clicked );

  // QgsOptionsDialogBase handles saving/restoring of geometry, splitter and current tab states,
  // switching vertical tabs between icon/text to icon-only modes (splitter collapsed to left),
  // and connecting QDialogButtonBox's accepted/rejected signals to dialog's accept/reject slots
  initOptionsBase( false );

  connect( buttonBox, &QDialogButtonBox::helpRequested, this, &QgsMeshLayerProperties::showHelp );
  connect( lyr->styleManager(), &QgsMapLayerStyleManager::currentStyleChanged, this, &QgsMeshLayerProperties::syncToLayer );

  connect( this, &QDialog::accepted, this, &QgsMeshLayerProperties::apply );
  connect( this, &QDialog::rejected, this, &QgsMeshLayerProperties::onCancel );

  connect( buttonBox->button( QDialogButtonBox::Apply ), &QAbstractButton::clicked, this, &QgsMeshLayerProperties::apply );

  if ( !mMeshLayer )
  {
    return;
  }

  // update based on lyr's current state
  sync();

  QgsSettings settings;
  // if dialog hasn't been opened/closed yet, default to Styles tab, which is used most often
  // this will be read by restoreOptionsBaseUi()
  if ( !settings.contains( QStringLiteral( "/Windows/MeshLayerProperties/tab" ) ) )
  {
    settings.setValue( QStringLiteral( "Windows/MeshLayerProperties/tab" ),
                       mOptStackedWidget->indexOf( mOptsPage_Style ) );
  }

  QString title = QString( tr( "Layer Properties - %1" ) ).arg( lyr->name() );

  if ( !mMeshLayer->styleManager()->isDefault( mMeshLayer->styleManager()->currentStyle() ) )
    title += QStringLiteral( " (%1)" ).arg( mMeshLayer->styleManager()->currentStyle() );
  restoreOptionsBaseUi( title );
}

/**
  \note moved from ctor

  Previously this dialog was created anew with each right-click pop-up menu
  invocation.  Changed so that the dialog always exists after first
  invocation, and is just re-synchronized with its layer's state when
  re-shown.

*/
void QgsMeshLayerProperties::sync()
{
  Q_ASSERT( mRendererMeshPropertiesWidget );

  QgsSettings myQSettings;

  QgsDebugMsg( "populate general information tab" );
  /*
   * Information Tab
   */
  QString info;
  if ( mMeshLayer && mMeshLayer->dataProvider() )
  {
    info += QStringLiteral( "<table>" );
    info += QStringLiteral( "<tr><td>%1</td><td>%2</td><tr>" ).arg( tr( "Uri" ) ).arg( mMeshLayer->dataProvider()->dataSourceUri() );
    info += QStringLiteral( "<tr><td>%1</td><td>%2</td><tr>" ).arg( tr( "Vertex count" ) ).arg( mMeshLayer->dataProvider()->vertexCount() );
    info += QStringLiteral( "<tr><td>%1</td><td>%2</td><tr>" ).arg( tr( "Face count" ) ).arg( mMeshLayer->dataProvider()->faceCount() );
    info += QStringLiteral( "<tr><td>%1</td><td>%2</td><tr>" ).arg( tr( "Dataset count" ) ).arg( mMeshLayer->dataProvider()->datasetCount() );
    info += QStringLiteral( "</table>" );
  }

  mInformationTextBrowser->setText( info );

  QgsDebugMsg( "populate source tab" );
  /*
   * Source Tab
   */
  mLayerOrigNameLineEd->setText( mMeshLayer->name() );
  leDisplayName->setText( mMeshLayer->name() );

  if ( mMeshLayer && mMeshLayer->dataProvider() )
  {
    mUriLabel->setText( mMeshLayer->dataProvider()->dataSourceUri() );
  }
  else
  {
    mUriLabel->setText( tr( "Not assigned" ) );
  }

  QgsDebugMsg( "populate styling tab" );
  /*
   * Styling Tab
   */
  mRendererMeshPropertiesWidget->syncToLayer();

} // QgsMeshLayerProperties::sync()

/*
 *
 * PUBLIC AND PRIVATE SLOTS
 *
 */
void QgsMeshLayerProperties::mAddDatasetButton_clicked()
{
  if ( !mMeshLayer || !mMeshLayer->dataProvider() )
    return;

  QString fileName = QFileDialog::getOpenFileName( this );
  if ( !fileName.isEmpty() )
  {
    bool ok = mMeshLayer->dataProvider()->addDataset( fileName );
    QgsDebugMsg( QStringLiteral( "Dataset added %1, %2" ).arg( fileName ).arg( ok ) );
    if ( ok )
      sync();
  }
}

void QgsMeshLayerProperties::apply()
{
  Q_ASSERT( mRendererMeshPropertiesWidget );

  QgsDebugMsg( "processing general tab" );
  /*
   * General Tab
   */
  mMeshLayer->setName( mLayerOrigNameLineEd->text() );

  QgsDebugMsg( "processing style tab" );
  /*
   * Style Tab
   */
  mRendererMeshPropertiesWidget->apply();

  //make sure the layer is redrawn
  mMeshLayer->triggerRepaint();

  // notify the project we've made a change
  QgsProject::instance()->setDirty( true );
}

void QgsMeshLayerProperties::onCancel()
{

}

void QgsMeshLayerProperties::mCrsSelector_crsChanged( const QgsCoordinateReferenceSystem &crs )
{
  if ( mMeshLayer )
    mMeshLayer->setCrs( crs );
}

void QgsMeshLayerProperties::mLayerOrigNameLineEd_textEdited( const QString &text )
{
  leDisplayName->setText( mMeshLayer->formatLayerName( text ) );
}

void QgsMeshLayerProperties::syncToLayer()
{
  sync();
  mMeshLayer->triggerRepaint();
}

void QgsMeshLayerProperties::showHelp()
{
  QgsHelp::openHelp( QStringLiteral( "working_with_mesh/mesh_properties.html" ) );
}
