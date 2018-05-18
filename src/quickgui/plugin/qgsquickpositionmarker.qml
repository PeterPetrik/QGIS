/***************************************************************************
  qgsquickpositionmarker.qml
  --------------------------------------
  Date                 : Dec 2017
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

import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQml 2.2
import QtGraphicalEffects 1.0
import QgsQuick 0.1 as QgsQuick

/**
 * \brief A marker refers to the current gps position according gps source is available. Along with longitude, latitude and radius.
 */
Item {
  id: positionMarker
  property int size: 48 * QgsQuick.Utils.dp

  /**
   * Note: required to be connected from parent!
   */
  property QgsQuick.MapSettings mapSettings
  /**
   * Utils for handling position.
   */
  property QgsQuick.PositionKit positionKit: QgsQuick.PositionKit
  {id: positionKit
    mapSettings: mapSettings
    simulatePositionLongLatRad: [-97.36, 36.93, 2]
    onProjectedPositionChanged: update_location()
  }
  /**
   * use in debug mode to simulate movement around some GPS location
   * longitude, latitude, and radius, all in degrees WSG84
   * e.g. simulatePositionLongLatRad = [60, 10, 0.02].
   */
  property var simulatePositionLongLatRad

  /**
   * Position in pixels
   */
  property point screenPosition
  /**
   * Color of the marker when gps is active.
   */
  property color baseColor: "darkblue"
  /**
   * Color of the marker when gps signal is lost.
   */
  property color unavailableColor: "gray"

  /**
   * Position in map coordinates
   * READ ONLY property
   */
  property alias mapPosition: positionKit.projectedPosition
  /**
   * Source position read from positionKit in WGS84 coordinates
   */
  property alias sourcePosition: positionKit.position

  //property alias rawPositionCRS: positionKit.pos

  /**
   * Source accuracy read from positionKit
   */
  property alias sourceAccuracy: positionKit.accuracy // in meters

  /**
   * Source accuracy units, default in meters.
   */
  property alias sourceAccuracyUnits: positionKit.accuracyUnits

  /**
   * Accuracy radius is active.
   */
  property var withAccuracy: true

  /**
   * Icon for position marker.
   */
  property var markerIcon: QgsQuick.Utils.getThemeIcon("ic_navigation_black")

  onMapSettingsChanged: update_location()

  Connections {
    target: mapSettings
    onVisibleExtentChanged: update_location()
  }

  function update_location() {
    if (mapSettings)
      screenPosition = mapSettings.coordinateToScreen(positionKit.projectedPosition)
  }

  /**
   * GPS accuracy circle-shaped indicator around positionMarker.
   */
  Rectangle {
    id: accuracyIndicator
    visible: mapSettings &&
             withAccuracy &&
             positionKit.hasPosition &&
             (positionKit.accuracy > 0) &&
             (accuracyIndicator.width > positionMarker.size / 2.0)
    x: positionMarker.screenPosition.x - width/2
    y: positionMarker.screenPosition.y - height/2
    width:positionKit.screenAccuracy
    height: accuracyIndicator.width
    color: baseColor
    border.color: "black"
    border.width: 3
    radius: width*0.5
    opacity: 0.1
  }

  /**
   * Position marker.
   */
  Rectangle {
    id: navigationMarker
    property int borderWidth: 2
    width: positionMarker.size + 20
    height: width
    color: "white"
    border.color: baseColor
    border.width: borderWidth
    radius: width*0.5
    antialiasing: true
    x: positionMarker.screenPosition.x - width/2
    y: positionMarker.screenPosition.y - height/2

    Image {
      id: navigation
      source: positionMarker.markerIcon
      fillMode: Image.PreserveAspectFit
      rotation: positionKit.direction
      anchors.centerIn: parent
      width: positionMarker.size
      height: width
    }

    /**
     * Makes positionMarker (navigation) grey if gps signal is lost.
     */
    ColorOverlay {
      anchors.fill: navigation
      source: navigation
      color: positionKit.hasPosition ? baseColor : unavailableColor
      rotation: positionKit.direction
      visible: !(positionKit.hasPosition && baseColor == "black")
    }
  }

}
