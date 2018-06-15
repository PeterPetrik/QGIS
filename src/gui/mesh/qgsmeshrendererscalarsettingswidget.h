/***************************************************************************
    qgsmeshrendererscalarsettingswidget.h
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

#ifndef QGSMESHRENDERERSCALARSETTINGSWIDGET_H
#define QGSMESHRENDERERSCALARSETTINGSWIDGET_H

#include "ui_qgsmeshrendererscalarsettingswidgetbase.h"

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
class GUI_EXPORT QgsMeshRendererScalarSettingsWidget : public QWidget, private Ui::QgsMeshRendererScalarSettingsWidgetBase
{
    Q_OBJECT

  public:

    /**
     * A widget to hold the renderer scalar settings for a mesh layer.
     * \param parent Parent object
     */
    QgsMeshRendererScalarSettingsWidget( QWidget *parent = nullptr );
    ~QgsMeshRendererScalarSettingsWidget();

    void setLayer( QgsMeshLayer *layer );

    QgsMeshRendererScalarSettings settings() const;

    void syncToLayer();

  signals:
    void widgetChanged();

  public slots:
    void setActiveDataset( int activeDatase );

  private slots:
    void refreshAfterStyleChanged();


    void minMaxChanged();
    void minMaxEdited();
    void recalculateMinMaxButtonClicked();

  private:
    double lineEditValue( const QLineEdit *lineEdit ) const;
    void calcMinMax( int datasetIndex, double &min, double &max ) const;

    QgsMeshLayer *mMeshLayer = nullptr;

    int mActiveDataset = -1;
};

#endif // QGSMESHRENDERERSCALARSETTINGSWIDGET_H
