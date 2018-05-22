/***************************************************************************
  qgsquickfeature.h
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

#ifndef QGSQUICKFEATURE_H
#define QGSQUICKFEATURE_H

#include <QObject>

#include "qgsfeature.h"

#include "qgis_quick.h"

class QgsVectorLayer;

/**
 * \ingroup quick
 * Helper class for QgsFeature and QgsVectorLayer where it belongs.
 *
 * \note QML Type: IdentifyResult
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickFeature
{
    Q_GADGET

    /**
     * Vector layer to which the feature belongs. This is a readonly property.
     */
    Q_PROPERTY( QgsVectorLayer *layer READ layer )
    /**
     * Feature instance itself. This is a readonly property.
     */
    Q_PROPERTY( QgsFeature feature READ feature )
    /**
     * Whether the feature is valid. This is a readonly property.
     */
    Q_PROPERTY( bool valid READ valid )

  public:
    //! Constructor of the new feature.
    QgsQuickFeature();

    /**
     * Constructor of the new feature.
     * \param feature New feature itself.
     * \param layer Vector layer which the feature belongs to.
     */
    QgsQuickFeature( const QgsFeature &feature,
                     QgsVectorLayer *layer );

    //! \copydoc QgsQuickFeature::layer
    QgsVectorLayer *layer() const;

    //! \copydoc QgsQuickFeature::feature
    QgsFeature feature() const;

    //! \copydoc QgsQuickFeature::valid
    bool valid() const;

  private:
    QgsVectorLayer *mLayer;
    QgsFeature mFeature;
};

Q_DECLARE_METATYPE( QgsQuickFeature )

#endif // QGSQUICKFEATURE_H
