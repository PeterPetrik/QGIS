/***************************************************************************
  qgsquickscalebar.qml
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
import QtQuick 2.7
import QtQuick.Controls 2.2
import QgsQuick 0.1 as QgsQuick

/**
 * \qmltype ScaleBar
 * \brief A component that shows scale ratio between its length and distance on the MapCanvas
 */
Item {
    id: scaleBar
    property alias mapSettings: scaleBarKit.mapSettings
    /**
     * Preferred width of scalebar in pixels. Defaults set to 300.
     */
    property alias preferredWidth: scaleBarKit.preferredWidth
    /**
     * Utility used for Scalebar properties.
     */
    property QgsQuick.ScaleBarKit scaleBarKit: QgsQuick.ScaleBarKit {id: scaleBarKit}


    /**
     * Reserved text width.
     */
    property int textWidth: fontMetrics.averageCharacterWidth * 8
    /**
     * Opacity of ScaleBar.
     */
    property color barColor: "white"
    /**
     * Background color of scaleBar.
     */
    property color barBackgroundColor: "grey"
    /**
     * Opacity of ScaleBar.
     */
    property double barOpacity: 0.8
    /**
     * Defines text shown on ScaleBar.
     */
    property string barText: scaleBarKit.distance + " " + scaleBarKit.units

    /**
     * Returns calculated width in pixels.
     */
    property int barWidth: scaleBarKit.width
    property int lineWidth: 5 * QgsQuick.Utils.dp

    /**
     * The real width of the component.
     */
    width: textWidth + barWidth

    MouseArea {
        anchors.fill: background
        onClicked: {
            animation.restart()
        }
    }

    NumberAnimation {
        id: animation
        target: scaleBar
        property: "barWidth"
        to: 200
        duration: 1000
    }

    Rectangle {
        id: background
        color: scaleBar.barBackgroundColor
        opacity: scaleBar.barOpacity
        width: parent.width
        height: parent.height
    }

    FontMetrics {
        id: fontMetrics
        font: text.font
    }

    Row {
        opacity: 1
        spacing: 0

        Text {
            id: text
            width: textWidth
            height: scaleBar.height
            text: barText
            color: barColor
            font.pixelSize: scaleBar.height - 2 * scaleBar.lineWidth
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Rectangle {
            id: leftBar
            width: scaleBar.lineWidth
            height: scaleBar.height - 20 * QgsQuick.Utils.dp
            y: (scaleBar.height - leftBar.height) / 2
            color: barColor
            opacity: 1
        }

        Rectangle {
            width: scaleBar.width - text.width - 15 * QgsQuick.Utils.dp
            height: scaleBar.lineWidth
            y: (scaleBar.height - scaleBar.lineWidth) / 2
            color: barColor
        }

        Rectangle {
            id: rightBar
            width: scaleBar.lineWidth
            height: scaleBar.height - 20 * QgsQuick.Utils.dp
            y: (scaleBar.height - leftBar.height) / 2
            color: barColor
        }
    }
}
