/***************************************************************************
    qgsmeshrenderervectorsettingswidget.h
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

#ifndef QGSMESHRENDERERVECTORSETTINGSWIDGET_H
#define QGSMESHRENDERERVECTORSETTINGSWIDGET_H

#include "ui_qgsmeshrenderervectorsettingswidgetbase.h"

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include "qgis_gui.h"
#include <memory>
#include "qgsmeshrenderersettings.h"

class QgsMeshLayer;

/**
 * \ingroup gui
 * \class QgsMeshRendererVectorSettingsWidget
 */
class GUI_EXPORT QgsMeshRendererVectorSettingsWidget : public QWidget, private Ui::QgsMeshRendererVectorSettingsWidgetBase
{
    Q_OBJECT

  public:

    /**
     * A widget to hold the renderer Vector settings for a mesh layer.
     * \param parent Parent object
     */
    QgsMeshRendererVectorSettingsWidget( QWidget *parent = nullptr );
    ~QgsMeshRendererVectorSettingsWidget();

    void setLayer( QgsMeshLayer *layer );

    QgsMeshRendererVectorSettings settings() const;

  signals:
    void widgetChanged();

  public slots:
    void setActiveDataset( int activeDataset );

  private slots:
    void refreshAfterStyleChanged();
    void syncToLayer();

  private:

    /**
     * convert text to double, return err_val if there is a problem
     * problem is also when the value is negative
     */
    double filterValue( const QString &text, double err_val ) const;

    QgsMeshLayer *mMeshLayer = nullptr;

    int mActiveDataset = -1;
};

#endif // QGSMESHRENDERERVECTORSETTINGSWIDGET_H
