/***************************************************************************
  qgsmeshlayerproperties.h
  ------------------------
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
#ifndef QGSMESHLAYERPROPERTIES_H
#define QGSMESHLAYERPROPERTIES_H

#include "qgsoptionsdialogbase.h"
#include "ui_qgsmeshlayerpropertiesbase.h"
#include "qgsguiutils.h"
#include "qgshelp.h"
#include "qgsmaplayerstylemanager.h"
#include "qgsmaptoolemitpoint.h"
#include "qgis_app.h"

class QgsPointXY;
class QgsMapLayer;
class QgsMapCanvas;
class QgsMeshLayer;
class QgsMetadataWidget;
class QgsRasterRenderer;
class QgsRasterRendererWidget;
class QgsRasterHistogramWidget;
class QgsRendererMeshPropertiesWidget;

/**
 * Property sheet for a raster map layer
  */
class APP_EXPORT QgsMeshLayerProperties : public QgsOptionsDialogBase, private Ui::QgsMeshLayerPropertiesBase
{
    Q_OBJECT

  public:

    /**
     * \brief Constructor
     * \param ml Map layer for which properties will be displayed
     */
    QgsMeshLayerProperties( QgsMapLayer *lyr, QgsMapCanvas *canvas, QWidget *parent = nullptr, Qt::WindowFlags = QgsGuiUtils::ModalDialogFlags );
    ~QgsMeshLayerProperties() = default;

    //! Synchronize state with associated mesh layer
    void sync();

  public slots:
    //TODO: Verify that these all need to be public
    //! \brief Applies the settings made in the dialog without closing the box
    void apply();
    //! Called when cancel button is pressed
    void onCancel();
    //! \brief Slot to update layer display name as original is edited.
    void mLayerOrigNameLineEd_textEdited( const QString &text );

  private slots:
    //! Help button
    void showHelp();

    //! Make GUI reflect the layer's state
    void syncToLayer();

    void mCrsSelector_crsChanged( const QgsCoordinateReferenceSystem &crs );
    void mAddDatasetButton_clicked();

  private:
    QgsRendererMeshPropertiesWidget *mRendererMeshPropertiesWidget = nullptr;

    //! \brief Pointer to the raster layer that this property dilog changes the behavior of.
    QgsMeshLayer *mMeshLayer = nullptr;
};


#endif // QGSMESHLAYERPROPERTIES_H
