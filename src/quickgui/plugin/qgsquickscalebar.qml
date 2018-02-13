import QtQuick 2.7
import QtQuick.Controls 2.2
import QgisQuick 0.1 as QgsQuick

Item {
    id: scaleBar
    property alias mapSettings: scaleBarKit.mapSettings
    property alias preferredWidth: scaleBarKit.preferredWidth

    QgsQuick.ScaleBarKit {
        id: scaleBarKit
    }

    property int textWidth: 50 * QgsQuick.Style.dp
    property color barColor: "white"
    property color barBackgroundColor: "grey"
    property double barOpacity: 0.8
    property string barText: scaleBarKit.distance + " " + scaleBarKit.units
    property int barWidth: scaleBarKit.width
    property int lineWidth: 5 * QgsQuick.Style.dp

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
            height: scaleBar.height - 20 * QgsQuick.Style.dp
            y: (scaleBar.height - leftBar.height) / 2
            color: barColor
            opacity: 1
        }

        Rectangle {
            width: scaleBar.width - text.width - 15 * QgsQuick.Style.dp
            height: scaleBar.lineWidth
            y: (scaleBar.height - scaleBar.lineWidth) / 2
            color: barColor
        }

        Rectangle {
            id: rightBar
            width: scaleBar.lineWidth
            height: scaleBar.height - 20 * QgsQuick.Style.dp
            y: (scaleBar.height - leftBar.height) / 2
            color: barColor
        }
    }
}
