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


Item {
    property alias project: layerTreeModel.project

    height: layerTreeView.contentHeight

    LayerTreeModel {
        id: layerTreeModel
    }

    Component {
        id: layerTreeItemDelegate
        Rectangle {
          id: wrapper
          width: 90
          height: info.height
          color: ListView.isCurrentItem ? "lightsteelblue" : "transparent"
          Text {
            id: info
            text: name
          }
        }
    }

    ListView {
        id: layerTreeView
        spacing: 9
        contentHeight: 300
        anchors.fill: parent
        model: layerTreeModel
        delegate: layerTreeItemDelegate
        focus: true
    }
}
