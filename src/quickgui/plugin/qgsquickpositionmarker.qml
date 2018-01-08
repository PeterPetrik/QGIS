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
import QgisQuick 1.0 as QgsQuick

Item {
    id: positionMarker
    property int size: 48 * QgsQuick.Style.dp

    property QgsQuick.MapSettings mapSettings // required to be connected from parent!

    property var simulatePositionLongLatRad // use in debug mode to simulate movement around some GPS location
                                            // longitude, latitude, and radius, all in degrees WSG84
                                            // e.g. simulatePositionLongLatRad = [60, 10, 0.02]

    property point screenPosition  // in pixels
    property alias mapPosition: wgs84toMapCrs.projectedPosition // in map coordinates
    property alias gpsPosition: positionKit.position // in WGS84 coordinates
    property alias positionKit: positionKit
    property var withAccuracy: true

    onMapSettingsChanged: update_location()
    onGpsPositionChanged: update_location()

    Connections {
        target: mapSettings
        onVisibleExtentChanged: update_location()
    }

    function update_location() {
        if (mapSettings)
            screenPosition = mapSettings.coordinateToScreen(wgs84toMapCrs.projectedPosition)
    }

    //TODO maybe create function to parse project layers (or mapsettings) and their extent and calculate some reasonable
    // values instead of hardcoding them in the application?
    // function calculateSimulatedPositionParams(project) { ... }

    onSimulatePositionLongLatRadChanged: {
        if (simulatePositionLongLatRad) {
            var longitude = simulatePositionLongLatRad[0]
            var latitude = simulatePositionLongLatRad[1]
            var radius = simulatePositionLongLatRad[2]
            console.log("Use simulated position around longlat: " + longitude + ", " + latitude + ", radius " + radius )
            positionKit.use_simulated_location(longitude, latitude, radius);
        } else {
            positionKit.use_gps_location();
        }
    }

    QgsQuick.CoordinateTransformer {
        id: wgs84toMapCrs
        sourceCrs: QgsQuick.Utils.coordinateReferenceSystemFromEpsgId(4326)
        destinationCrs: mapSettings.destinationCrs
        sourcePosition: positionKit.position
    }

    QgsQuick.PositionKit {
        id: positionKit
    }

    Rectangle {
        id: accuracyIndicator
        visible: withAccuracy
        x: positionMarker.screenPosition.x - width/2
        y: positionMarker.screenPosition.y - height/2
        width: {
            if (positionKit.accuracy > 0)
                QgsQuick.Utils.distanceToMapUnits(mapSettings, positionKit.accuracy, 0, 0, 0) * 2
            else positionMarker.size
        }
        height: accuracyIndicator.width
        color: "red"
        border.color: "black"
        border.width: 1
        radius: width*0.5
        opacity: 0.3
    }

    Image {
        id: navigation
        source: QgsQuick.Utils.getThemeIcon("ic_navigation_black_48px")
        fillMode: Image.PreserveAspectFit
        rotation: positionKit.direction
        x: positionMarker.screenPosition.x - width/2
        y: positionMarker.screenPosition.y - height/2
        width: positionMarker.size
        height: width
    }

    ColorOverlay {
            anchors.fill: navigation
            source: navigation
            color: "grey"
            rotation: positionKit.direction
            visible: !positionKit.hasPosition
        }
}
