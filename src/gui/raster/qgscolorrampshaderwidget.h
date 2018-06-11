/***************************************************************************
                         qgscolorrampshaderwidget.h
                         --------------------------
    begin                : Jun 2018 by Peter Petrik
    copyright            : (C) 2012 by Marco Hugentobler
    email                : marco at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSCOLORRAMPSHADERWIDGET_H
#define QGSCOLORRAMPSHADERWIDGET_H

#include "qgis_sip.h"
#include "qgscolorrampshader.h"
#include "qgsrasterrenderer.h"
#include "ui_qgscolorrampshaderwidgetbase.h"
#include "qgis_gui.h"
#include "qgsrasterrendererwidget.h"

class QgsRasterDataProvider;

/**
 * \ingroup gui
 * \class QgsColorRampShaderWidget
 *
 * It has 2 ways how to use it. For raster layers, raster data provider and band is assigned and
 * the Quantile classification mode can be used and the LoadFromBandButton is visible.
 *
 * The other mode is used to style mesh layer contours (scalar datasets)
 */
class GUI_EXPORT QgsColorRampShaderWidget: public QWidget, protected Ui::QgsColorRampShaderWidgetBase
{

    Q_OBJECT

  public:

    QgsColorRampShaderWidget( QWidget *parent = nullptr );

    void initForUseWithRasterLayer();

    void setRasterBand( QgsRasterDataProvider *dp, int band, const QgsRectangle &extent );
    void setMinMaxAndClassify( double min, double max );
    void setMinMax( double min, double max );

    //! Returns shared function used in the renderer. Caller takes ownership and deletes it.
    QgsColorRampShader *shader() const;
    void setFromShader( const QgsColorRampShader *colorRampShader );

  signals:
    void minMaxChangedFromTree( double min, double max );
    void widgetChanged();
    void classificationModeChanged( QgsColorRampShader::ClassificationMode mode );

  public slots:

    /**
     * Executes the single band pseudo raster classification
     */
    void classify();

    //! called when the color ramp tree has changed
    void loadMinMaxFromTree();

  protected:
    void populateColormapTreeWidget( const QList<QgsColorRampShader::ColorRampItem> &colorRampItems );

  private:

    enum Column
    {
      ValueColumn = 0,
      ColorColumn = 1,
      LabelColumn = 2,
    };

    /**
     * Generate labels from the values in the color map.
     *  Skip labels which were manually edited (black text).
     *  Text of generated labels is made gray
     */
    void autoLabel();

    //! Extract the unit out of the current labels and set the unit field.
    void setUnitFromLabels();

    QMenu *contextMenu = nullptr;

  private slots:

    void applyColorRamp();
    void mAddEntryButton_clicked();
    void mDeleteEntryButton_clicked();
    void mLoadFromBandButton_clicked();
    void mLoadFromFileButton_clicked();
    void mExportToFileButton_clicked();
    void mUnitLineEdit_textEdited( const QString &text ) { Q_UNUSED( text ); autoLabel(); }
    void mColormapTreeWidget_itemDoubleClicked( QTreeWidgetItem *item, int column );
    void mColormapTreeWidget_itemEdited( QTreeWidgetItem *item, int column );
    //void bandChanged();
    void mColorInterpolationComboBox_currentIndexChanged( int index );
    //void mMinLineEdit_textChanged( const QString & ) { resetClassifyButton(); }
    //void mMaxLineEdit_textChanged( const QString & ) { resetClassifyButton(); }
    //void mMinLineEdit_textEdited( const QString &text );
    //void mMaxLineEdit_textEdited( const QString &text );
    void mClassificationModeComboBox_currentIndexChanged( int index );
    void changeColor();
    void changeOpacity();

  private:

    void setLineEditValue( QLineEdit *lineEdit, double value );
    double lineEditValue( const QLineEdit *lineEdit ) const;
    void resetClassifyButton();

    double mMin = std::numeric_limits<double>::quiet_NaN();
    double mMax = std::numeric_limits<double>::quiet_NaN();

    // For mode with raster layer
    QgsRasterDataProvider *mRasterDataProvider = nullptr;
    int mBand = -1;
    QgsRectangle mExtent;

};

#endif // QGSCOLORRAMPSHADERWIDGET_H
