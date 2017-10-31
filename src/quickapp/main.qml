import QtQuick 2.4
import QtQuick.Window 2.2
import QgisQuick 1.0 as QgsQuick

import QtQuick.Controls 1.3

Window {
    property string projectFile

    visible: true

    width: 800
    height: 600


    Component.onCompleted: {
        QgsQuick.Project.projectFile = Qt.binding(function() { return projectFile })
      //  canvas.engine.layers = Qt.binding(function() { return QgsQuick.Project.layers })
    }

    QgsQuick.MapCanvas {
        id: canvas
        anchors.fill: parent
        engine.destinationCRS: "EPSG:4326" //"EPSG:3857"  // pseudo Mercator projection
    }

}
