/***************************************************************************
  qgsquickscalebarkit.h
  --------------------------------------
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

#ifndef QGSQUICKSCALEBARKIT_H
#define QGSQUICKSCALEBARKIT_H

#include "qgis_quick.h"

#include <QObject>
#include <QString>

class QgsQuickMapSettings;
class QgsDistanceArea;

/**
 * \ingroup quick
 *
 * The class QgsQuickScaleBarKit encapsulates the utilies to calculate
 * scale bar properties
 *
 * It requires connection to mapSettings of the active canvas to automatically
 * update text and width
 *
 * From preferred width in pixel, it calculates the width (pixel) of scalebar
 * distance in meters or kilometers (int) rounded to "nice" number (e.g. 72.4 to 100)
 * and units text (e.g. km)
 *
 * \since QGIS 3.2
 */
class QUICK_EXPORT QgsQuickScaleBarKit : public QObject
{
  Q_OBJECT

  Q_PROPERTY( int preferredWidth MEMBER mPreferredWidth NOTIFY preferredWidthChanged)
  Q_PROPERTY( QgsQuickMapSettings* mapSettings MEMBER mMapSettings WRITE setMapSettings NOTIFY mapSettingsChanged)

  Q_PROPERTY( QString units READ units NOTIFY scaleBarChanged )
  Q_PROPERTY( int distance READ distance NOTIFY scaleBarChanged )
  Q_PROPERTY( int width READ width NOTIFY scaleBarChanged )

public:
  explicit QgsQuickScaleBarKit(QObject *parent = 0);
  ~QgsQuickScaleBarKit();

  void setMapSettings(QgsQuickMapSettings* mapSettings);
  int width() const;
  int distance() const;
  QString units() const;

signals:
  void scaleBarChanged();
  void mapSettingsChanged();
  void preferredWidthChanged();

public slots:
  void updateScaleBar();

private:
  double screenUnitsToMeters() const;

  QgsQuickMapSettings* mMapSettings;
  QgsDistanceArea* mDistanceArea;

  int mPreferredWidth; // pixels
  int mWidth; // pixels
  int mDistance; // in meters or kilometers, rounded
  QString mUnits; // km or m
};


#endif // QGSQUICKSCALEBARKIT_H
