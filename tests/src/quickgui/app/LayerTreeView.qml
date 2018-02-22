/***************************************************************************
  LayerTreeView.qml
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

import QtQuick 2.9
import QtQuick.Controls 2.2
import QgisQuick 0.1 as QgsQuick
//import '.' as QgsQuickApp
import QgisQuickApp 1.0



Item {
    property alias project: layerTreeModel.project
    property var currentLayer

    height: layerTreeView.contentHeight

    Rectangle {
        width: 400
        height: 400
        color: "red"
    }

    Rectangle {
        id: background
        width: column.width
        height: column.height
        color: "white"
    }

    LayerTreeModel {
        id: layerTreeModel
    }

    Column {
        id: column
        Repeater {
            model: layerTreeModel
            RadioDelegate {
                checked: index == 0
                text: name
                onCheckedChanged: {
                    if (checked) {
                        currentLayer = layerTreeModel.data(layerTreeModel.index(index), LayerTreeModel.VectorLayer);
                        console.log("active layer:", text, "(", currentLayer, ")");
                    }
                }
            }
        }
    }
}
