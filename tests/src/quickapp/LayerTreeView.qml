/***************************************************************************
  qgsquicklayertreemodel.qml
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

import QtQuick 2.0
import QgisQuickApp 1.0


Rectangle {
    property alias project: layerTreeModel.project
    opacity: 0.7

    LayerTreeModel {
        id: layerTreeModel
    }

    Component {
        id: layerTreeItemDelegate
        Item {
            width: 180; height: 40
            Column {
                Text { text: '<b>Name:</b> ' + name }
            }
        }
    }

    ListView {
        id: layerTreeView
        anchors.fill: parent

        model: layerTreeModel

        delegate: layerTreeItemDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
        focus: true
    }
}
