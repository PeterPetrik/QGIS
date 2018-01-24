/***************************************************************************
 qgsquickmessagelog.qml
  --------------------------------------
  Date                 : January 2018
  Copyright            : (C) 2018 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick.Controls 2.0
import QtQuick 2.5
import QgisQuick 0.1 as QgsQuick

Item {
  property alias model: table.model

  id: item

  Rectangle {
    color: "white"
    anchors.fill: parent
  }

  ListView {
    id: table
    anchors.fill: parent

    delegate: Column {
      Text {
        text: MessageTag
        font.bold: true
      }

      Text {
        text: Message
        width: table.width
        wrapMode: Text.WordWrap
      }

      Rectangle {
        color: "gray"
        height: 1*QgsQuick.Style.dp
      }
    }
  }
}
