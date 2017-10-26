import QtQuick 2.4
import QtQuick.Window 2.2
import qgis 1.0

import QtQuick.Controls 1.3

Window {
    property string projectFile

    visible: true

    width: 800
    height: 600


    Component.onCompleted: {
        Project.projectFile = Qt.binding(function() { return projectFile })
        canvas.engine.layers = Qt.binding(function() { return Project.layers })
        // canvas.engine.view.center = canvas.engine.fullExtent().center;
    }

    MapCanvas {
        id: canvas
        anchors.fill: parent
        engine.destinationCRS: "EPSG:4326" //"EPSG:3857"  // pseudo Mercator projection


        PositionAccuracy { }
        PositionMarker { }
    }

    Rectangle {

        anchors.top: canvas.top
        anchors.left: canvas.left
        anchors.margins: 8

        radius: width/2

        color:  mousearea.pressed ? "#44000000" : "#88000000"
        width: btnLocation.width * 1.2
        height: width

        Image {
            id: btnLocation
            anchors.centerIn: parent
            source: "assets:/location.png"
            MouseArea {
                id: mousearea
                anchors.fill: btnLocation
                onClicked: {
                    canvas.view.center = PositionEngine.position
                    canvas.view.mupp = canvas.engine.scale2mupp(500000)
                }
            }
        }
    }

    ScaleBar {
        id: scaleBar

        engine: canvas.engine

        anchors.bottom: canvas.bottom
        anchors.right: canvas.right
        anchors.bottomMargin: 8
        anchors.rightMargin: 8
    }
}
