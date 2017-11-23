/***************************************************************************
 qgsquickbutton.qml
  --------------------------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick.Controls 2.2
import QtQuick 2.5
import QgisQuick 1.0 as QgsQuick

Item {
  id: item

  property alias iconSource: button.iconSource
  property alias checked: button.checked
  property alias checkable: button.checkable
  property bool round: false
  property alias text: button.text

  property color bgcolor: "#212121"
  property color borderColor: bgcolor

  signal pressAndHold
  signal clicked

  function rotate() {
    rotation.angle = rotation.angle ? 0 : 180
  }

  height: 48 * QgsQuick.Style.dp
  width: 48 * QgsQuick.Style.dp

  Button {
    id: button
    anchors.fill: parent

    /* TODO
    style: ButtonStyle {
      background: Rectangle {
        implicitWidth: 100
        implicitHeight: 25
        border.width: !round
        border.color: borderColor
        color: bgcolor
        radius: round ? height / 2 : 0

        Behavior on color {
          ColorAnimation {
            duration: 200
          }
        }
      }
    }
    */

    MouseArea {
      anchors.fill: parent

      onClicked: item.clicked()
      onPressAndHold: item.pressAndHold()
    }
  }

  transform: Rotation {
    id: rotation
    origin.x: width / 2; origin.y: height / 2;

    Behavior on angle {
      NumberAnimation {
        duration: 200
      }
    }
  }
}
