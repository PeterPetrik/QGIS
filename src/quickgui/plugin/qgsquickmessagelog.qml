import QtQuick.Controls 1.4
import QtQuick 2.5
import QgisQuick 1.0 as QgsQuick

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
        wrapMode: Text.WordWrap
      }

      Rectangle {
        color: "gray"
        height: 1*QgsQuick.Style.dp
      }
    }
  }
}
