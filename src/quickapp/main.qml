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
import QgisQuick 1.0 as QgsQuick

Window {
    property string projectFile

    visible: true

    width: 800
    height: 600

    Component.onCompleted: {
        console.log("*** ALL LOADED! ***")
    }


    QgsQuick.Project {
        id: project
        fileName: projectFile
    }

    QgsQuick.MapCanvas {
        id: canvas
        anchors.fill: parent
        mapSettings.layers: project.layers
    }

    QgsQuick.LayerTreeView {
        id: layerTreeView
        anchors.fill: parent
        model: project.layerTreeModel
    }

}
