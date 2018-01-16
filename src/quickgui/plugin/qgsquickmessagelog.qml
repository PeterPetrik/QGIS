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
        wrapMode: Text.WordWrap
      }

      Rectangle {
        color: "gray"
        height: 1*QgsQuick.Style.dp
      }
    }
  }
}
