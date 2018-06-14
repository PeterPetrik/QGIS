/***************************************************************************
    qgsmeshrenderermeshsettingswidget.h
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

#ifndef QGSMESHRENDERERMESHSETTINGSWIDGET_H
#define QGSMESHRENDERERMESHSETTINGSWIDGET_H

#include "ui_qgsmeshrenderermeshsettingswidgetbase.h"

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include "qgis_gui.h"
#include <memory>
#include "qgsmeshrenderersettings.h"

class QgsMeshLayer;

/**
 * \ingroup gui
 * \class QgsMeshRendererMeshSettingsWidget
 */
class GUI_EXPORT QgsMeshRendererMeshSettingsWidget : public QWidget, private Ui::QgsMeshRendererMeshSettingsWidgetBase
{
    Q_OBJECT

  public:

    /**
     * A widget to hold the renderer Mesh settings for a mesh layer.
     * \param parent Parent object
     */
    QgsMeshRendererMeshSettingsWidget( QWidget *parent = nullptr );
    ~QgsMeshRendererMeshSettingsWidget();

    void setLayer( QgsMeshLayer *layer, bool isTriangularMesh );

    QgsMeshRendererMeshSettings settings() const;

  signals:
    void widgetChanged();

  private slots:
    void refreshAfterStyleChanged();
    void syncToLayer();

    void onWidthChanged( double value );

  private:
    QgsMeshLayer *mMeshLayer = nullptr;
    bool mIsTriangularMesh = true;
};

#endif // QGSMESHRENDERERMESHSETTINGSWIDGET_H
