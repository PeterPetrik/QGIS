/***************************************************************************
  main.qml
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

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QgisQuick 1.0 as QgsQuick
import '.' as QgsQuickApp

Window {

    visible: true

    width: 800
    height: 600

    Component.onCompleted: {
        console.log("*** ALL LOADED! ***")
    }

    QgsQuick.Project {
        id: project
        fileName: qgisProject
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 6

        QgsQuickApp.LayerTreeView {
            id: layerTreeView
            project: project

            Layout.fillHeight: true
            Layout.minimumWidth: 150
            Layout.maximumWidth: 300
        }

        QgsQuick.MapCanvas {
            id: mapCanvas
            mapSettings.layers: project.layers

            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    QgsQuick.ScaleBar {
        id: scaleBar
        preferredWidth: 300 * devicePixels
        mapSettings: mapCanvas.mapSettings

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
}
