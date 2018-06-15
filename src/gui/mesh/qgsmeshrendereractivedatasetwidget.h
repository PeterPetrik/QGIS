/***************************************************************************
    qgsmeshrendereractivedatasetwidget.h
    -------------------------------------
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

#ifndef QGSMESHRENDERERACTIVEDATASETWIDGET_H
#define QGSMESHRENDERERACTIVEDATASETWIDGET_H

#include "ui_qgsmeshrendereractivedatasetwidgetbase.h"

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include "qgis_gui.h"
#include <memory>
#include "qgsmeshrenderersettings.h"

class QgsMeshLayer;

/**
 * \ingroup gui
 * \class QgsMeshRendererScalarSettingsWidget
 */
class GUI_EXPORT QgsMeshRendererActiveDatasetWidget : public QWidget, private Ui::QgsMeshRendererActiveDatasetWidgetBase
{
    Q_OBJECT

  public:

    /**
     * A widget to hold the renderer scalar settings for a mesh layer.
     * \param parent Parent object
     */
    QgsMeshRendererActiveDatasetWidget( QWidget *parent = nullptr );
    ~QgsMeshRendererActiveDatasetWidget();

    void setLayer( QgsMeshLayer *layer );
    int activeScalarDataset() const;
    int activeVectorDataset() const;
    bool meshRenderingOn() const;
    bool triangularMeshRenderingOn() const;
    void syncToLayer();

  signals:
    void activeScalarDatasetChanged( int index );
    void activeVectorDatasetChanged( int index );
    void meshRenderingOnChanged( bool on );
    void triangularMeshRenderingOnChange( bool on );

    void widgetChanged();

  private slots:
    void onActiveGroupChanged();
    void onActiveDatasetChanged( int value );
    void onScalarChecked( int toggle );
    void onVectorChecked( int toggle );
    void onMeshChecked( int toggle );
    void onTringularMeshChecked( int toggle );

    void updateMetadata( int datasetIndex );

  private:
    int datasetIndex() const;


    QgsMeshLayer *mMeshLayer = nullptr;
};

#endif // QGSMESHRENDERERSCALARSETTINGSWIDGET_H
