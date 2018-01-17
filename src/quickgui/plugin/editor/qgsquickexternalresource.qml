import QtQuick 2.5
import QtQuick.Controls 2.0
import QgisQuick 0.1 as QgsQuick


Item {
  signal valueChanged(var value, bool isNull)

  property var image: image

  id: fieldItem
  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(image.height, button.height)

  Image {
    property var currentValue: value

    id: image
    width: 200 * QgsQuick.Style.dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit

    Component.onCompleted: image.source = getSource()

    function getSource() {
        if (image.status === Image.Error)
            return QgsQuick.Utils.getThemeIcon("ic_broken_image_black")
        else if (image.currentValue && QgsQuick.Utils.fileExists(homePath + "/" + image.currentValue)) {
            return homePath + "/" + image.currentValue
        }
        else       {
            return QgsQuick.Utils.getThemeIcon("ic_photo_notavailable_white")
        }
    }
  }

  Button {
    id: button
    width: 45 * QgsQuick.Style.dp
    height: 45 * QgsQuick.Style.dp

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    onClicked: {
        photoCapturePanel.visible = true
        photoCapturePanel.targetDir = homePath
        photoCapturePanel.fieldItem = fieldItem
    }

    background: Image {
        source: QgsQuick.Utils.getThemeIcon("ic_camera_alt_border")
        width: button.width
        height: button.height
    }
  }
}
