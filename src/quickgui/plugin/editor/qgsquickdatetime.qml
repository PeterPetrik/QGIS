import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1
import QgisQuick 1.0 as QgsQuick
import QtQuick.Controls 1.4 as Controls1
/*
  * field_format
  * display_format
  * calendar_popup
  * allow_null
 */


Item {
  signal valueChanged(var value, bool isNull)

  height: childrenRect.height
  anchors { right: parent.right; left: parent.left }

  ColumnLayout {
    id: main
    property var currentValue: value

    anchors { right: parent.right; left: parent.left }

    Item {
      anchors { right: parent.right; left: parent.left }
      Layout.minimumHeight: 48 * QgsQuick.Style.dp

      Rectangle {
        anchors.fill: parent
        id: backgroundRect
        border.color: comboBox.pressed ? "#17a81a" : "#21be2b"
        border.width: comboBox.visualFocus ? 2 : 1
        color: "#dddddd"
        radius: 2
      }

      Label {
        id: label

        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter

        MouseArea {
          anchors.fill: parent
          onClicked: {
            popup.open()
          }
        }

        Image {
          source: QgsQuick.Utils.getThemeIcon("ic_clear_black_18dp")
          anchors.left: parent.right
          visible: main.currentValue !== undefined && config['allow_null']

          MouseArea {
            anchors.fill: parent
            onClicked: {
              main.currentValue = undefined
            }
          }
        }
      }
    }

    Popup {
      id: popup
      modal: true
      focus: true
      closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
      parent: ApplicationWindow.overlay

      ColumnLayout {

        Controls1.Calendar {
          id: calendar
          selectedDate: currentValue
          weekNumbersVisible: true
          focus: false

          onSelectedDateChanged: {
            main.currentValue = selectedDate
          }
        }

        RowLayout {
          Button {
            text: qsTr( "Ok" )
            Layout.fillWidth: true

            onClicked: popup.close()
          }
        }
      }
    }

    onCurrentValueChanged: {
      valueChanged(currentValue, main.currentValue === undefined)
      if (main.currentValue === undefined)
      {
        label.text = qsTr('(no date)')
        label.color = 'gray'
      }
      else
      {
        label.color = 'black'
        label.text = new Date(value).toLocaleString(Qt.locale(), config['display_format'] )
      }
    }
  }
}
