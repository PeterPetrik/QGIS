/***************************************************************************
  qgsquickidentifyresult.cpp
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

#include "qgsvectorlayer.h"
#include "qgsfeature.h"

#include "qgsquickidentifyresult.h"

QgsQuickIdentifyResult::QgsQuickIdentifyResult( const QgsFeature& feature, QgsVectorLayer* layer)
  : mLayer( layer )
  , mFeature( feature )
{
    if (!mLayer) {
        mFeature.setValid(false);
    }
}

QgsQuickIdentifyResult::QgsQuickIdentifyResult(): mLayer(0)
{
    mFeature.setValid(false);
}

QgsVectorLayer* QgsQuickIdentifyResult::layer() const {
    return mLayer;
}

QgsFeature QgsQuickIdentifyResult::feature() const{
    return mFeature;
}

bool QgsQuickIdentifyResult::valid() const {
    return (mLayer && mFeature.isValid());
}
