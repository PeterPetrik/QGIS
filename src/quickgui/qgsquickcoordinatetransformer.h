/***************************************************************************
 qgsquickcoordinatetransform.h
  --------------------------------------
  Date                 : 1.6.2017
  Copyright            : (C) 2017 by Matthias Kuhn
  Email                :  matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKCOORDINATETRANSFORMER_H
#define QGSQUICKCOORDINATETRANSFORMER_H

#include "qgis_quick.h"

#include <QObject>

#include <qgspoint.h>

class QUICK_EXPORT QgsQuickCoordinateTransformer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY projectedPositionChanged )
    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition WRITE setSourcePosition NOTIFY sourcePositionChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem sourceCrs READ sourceCrs WRITE setSourceCrs NOTIFY sourceCrsChanged )

  public:
    explicit QgsQuickCoordinateTransformer( QObject *parent = 0 );
    QgsPoint projectedPosition() const;

    QgsPoint sourcePosition() const;
    void setSourcePosition( QgsPoint sourcePosition );

    QgsCoordinateReferenceSystem destinationCrs() const;
    void setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs );

    QgsCoordinateReferenceSystem sourceCrs() const;
    void setSourceCrs( const QgsCoordinateReferenceSystem& sourceCrs );

  private:
    void updatePosition();

  signals:
    void projectedPositionChanged();
    void sourcePositionChanged();
    void destinationCrsChanged();
    void sourceCrsChanged();

  private:
    QgsPoint mProjectedPosition;
    QgsPoint mSourcePosition;
    QgsCoordinateTransform mCoordinateTransform;
};

#endif // QGSQUICKCOORDINATETRANSFORMER_H
