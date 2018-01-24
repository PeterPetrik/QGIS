import QtQuick 2.0
import QtQuick.Controls 2.2
import QgisQuick 1.0 as QgsQuick

Item {
  signal valueChanged( var value, bool isNull )

  height: childrenRect.height
  anchors {
    right: parent.right
    left: parent.left
  }

  CheckBox {
    property var currentValue: value

    checked: value === config['CheckedState']

    onCheckedChanged: {
      valueChanged( checked ? config['CheckedState'] : config['UncheckedState'], false )
      forceActiveFocus()
    }

    // Workaround to get a signal when the value has changed
    onCurrentValueChanged: {
      checked = currentValue === config['CheckedState']
    }
  }
}
