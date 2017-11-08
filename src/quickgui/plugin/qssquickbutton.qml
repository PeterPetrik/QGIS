import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
import QtQuick 2.5

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

  height: 48 * dp
  width: 48 * dp

  Controls.Button {
    id: button
    anchors.fill: parent

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
