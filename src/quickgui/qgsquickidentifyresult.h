/***************************************************************************
  qgsquickidentifyresult.h
 ---------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKIDENTIFYRESULT_H
#define QGSQUICKIDENTIFYRESULT_H

#include <QObject>

#include "qgis_quick.h"
#include <qgsfeature.h>
class QgsVectorLayer;

class QUICK_EXPORT QgsQuickIdentifyResult
{
    Q_GADGET

    Q_PROPERTY( QgsVectorLayer* layer READ layer )
    Q_PROPERTY( QgsFeature feature READ feature )
    Q_PROPERTY( bool valid READ valid)

  public:
    QgsQuickIdentifyResult();
    QgsQuickIdentifyResult( const QgsFeature& feature,
                            QgsVectorLayer* layer);

    //QgsQuickIdentifyResult(const QgsQuickIdentifyResult& other);
    //QgsQuickIdentifyResult &operator=( const QgsQuickIdentifyResult &rhs );

    QgsVectorLayer* layer() const;
    QgsFeature feature() const;
    bool valid() const;

  private:
    QgsVectorLayer* mLayer;
    QgsFeature mFeature;
};

Q_DECLARE_METATYPE(QgsQuickIdentifyResult)

#endif // QGSQUICKIDENTIFYRESULT_H
