/***************************************************************************
    qgsrenderermeshpropertieswidget.h
    ---------------------
    begin                : June 2018
    copyright            : (C) 2018 by Peter Petrik
    email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSRENDERERMESHPROPERTIESDIALOG_H
#define QGSRENDERERMESHPROPERTIESDIALOG_H

#include <QObject>
#include <QDialog>

#include "ui_qgsrenderermeshpropswidgetbase.h"

#include "qgsmaplayerconfigwidget.h"
#include "qgis_gui.h"
#include <memory>

class QgsMeshLayer;
class QgsMapCanvas;

/**
 * \ingroup gui
 * \class QgsRendererMeshPropertiesWidget
 */
class GUI_EXPORT QgsRendererMeshPropertiesWidget : public QgsMapLayerConfigWidget, private Ui::QgsRendererMeshPropsWidgetBase
{
    Q_OBJECT

  public:

    /**
     * A widget to hold the renderer properties for a raster layer.
     * \param layer The mesh layer to style
     * \param canvas The canvas object used to calculate the max and min values from the extent.
     * \param parent Parent object
     */
    QgsRendererMeshPropertiesWidget( QgsMapLayer *layer, QgsMapCanvas *canvas, QWidget *parent = nullptr );
    ~QgsRendererMeshPropertiesWidget();

  public slots:
    void apply() override;
    void syncToLayer();

  private:
    QgsMeshLayer *mMeshLayer = nullptr;
};

#endif // QGSRENDERERMESHPROPERTIESDIALOG_H
