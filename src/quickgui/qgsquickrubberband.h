/***************************************************************************
 qgsquicksrubberband.h
  --------------------------------------
  Date                 : 11.6.2016
  Copyright            : (C) 2016 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKRUBBERBAND_H
#define QGSQUICKRUBBERBAND_H

#include <QQuickItem>
#include "qgis_quick.h"

class QgsQuickRubberbandModel;
class QgsQuickMapSettings;

class QUICK_EXPORT QgsQuickRubberband : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickRubberbandModel* model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( QgsQuickMapSettings* mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( qreal width READ width WRITE setWidth NOTIFY widthChanged )

  public:
    QgsQuickRubberband( QQuickItem* parent = nullptr );

    QgsQuickRubberbandModel* model() const;
    void setModel( QgsQuickRubberbandModel* model );

    QgsQuickMapSettings* mapSettings() const;
    void setMapSettings( QgsQuickMapSettings* mapSettings );

    QColor color() const;
    void setColor( const QColor& color );

    qreal width() const;
    void setWidth( qreal width );

  signals:
    void modelChanged();
    void mapSettingsChanged();
    void colorChanged();
    void widthChanged();

  private slots:
    void markDirty();

  private:
    QSGNode* updatePaintNode( QSGNode* n, QQuickItem::UpdatePaintNodeData* );

    QgsQuickRubberbandModel* mModel;
    QgsQuickMapSettings* mMapSettings;
    bool mDirty;
    QColor mColor;
    qreal mWidth;
};

#endif // QGSQUICKRUBBERBAND_H
