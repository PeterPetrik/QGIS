/***************************************************************************
  qgsquickscalebar.qml
  --------------------------------------
  Date                 : Nove 2017
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

import QtQuick 2.0
import QtQuick.Controls 2.2
import QgisQuick 1.0 as QgsQuick

Item {
    property alias mapSettings: scaleBarKit.mapSettings
    property alias preferredWidth: scaleBarKit.preferredWidth
    property int scaleBarLineWidth: 3  // line width of the scale bar
    property int scaleBarNotchHeight: 10 // height of the left and right notch
    property int labelBarMargin: 8 // how far is the label from the scale bar

    QgsQuick.ScaleBarKit {
        id: scaleBarKit
    }

    width: label.x + label.width
    height: label.height

    Rectangle {
        anchors.fill: parent
        color: "white"
    }

    Rectangle {
        id: rectBar
        anchors.bottom: label.baseline
        width: scaleBarKit.width  // length of the scale bar in pixels
        height: scaleBarLineWidth
        color: "black"

        Behavior on width {
          SmoothedAnimation { duration: 200 }
        }
    }

    Rectangle {
        id: leftBar
        anchors.left: rectBar.left
        anchors.bottom: rectBar.bottom
        width: scaleBarLineWidth
        height: scaleBarNotchHeight
        color: "black"
    }

    Rectangle {
        id: rightBar
        anchors.right: rectBar.right
        anchors.bottom: rectBar.bottom
        width: scaleBarLineWidth
        height: scaleBarNotchHeight
        color: "black"
    }

    Label {
        id: label
        text: scaleBarKit.distance + " " + scaleBarKit.units

        anchors.left: rectBar.right
        anchors.leftMargin: labelBarMargin
    }

}
