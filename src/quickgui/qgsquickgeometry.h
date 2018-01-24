/***************************************************************************
 qgsquickgeometry.h
  --------------------------------------
  Date                 : Nov 2017
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

#ifndef QGSQUICKGEOMETRY_H
#define QGSQUICKGEOMETRY_H

#include <QtPositioning/QGeoCoordinate>

#include "qgsgeometry.h"

#include "qgis_quick.h"
#include "qgsquickrubberbandmodel.h"

/**
 * \ingroup quick
 * Helper class to convert QgsQuickRubberbandModel to QgsGeometry
 *
 * \note QML Type: Not exported
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickGeometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickRubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( QgsVectorLayer *vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )

  public:
    explicit QgsQuickGeometry( QObject *parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    QgsQuickRubberbandModel *rubberbandModel() const;
    void setRubberbandModel( QgsQuickRubberbandModel *rubberbandModel );

    Q_INVOKABLE void applyRubberband();

    QgsVectorLayer *vectorLayer() const;
    void setVectorLayer( QgsVectorLayer *vectorLayer );

  signals:
    void rubberbandModelChanged();
    void vectorLayerChanged();

  private:
    QgsQuickRubberbandModel *mRubberbandModel;
    QgsVectorLayer *mVectorLayer;
};

#endif // QGSQUICKGEOMETRY_H
