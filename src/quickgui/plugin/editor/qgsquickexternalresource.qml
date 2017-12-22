import QtQuick 2.5
import QtQuick.Controls 2.0
import QgisQuick 1.0 as QgsQuick


Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(image.height, button.height)

  Image {
    property var currentValue: value

    id: image
    width: 200 * QgsQuick.Style.dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit

    source: {
      if (image.status === Image.Error)
        QgsQuick.Utils.getThemeIcon("ic_broken_image_black_24dp")
      else if (image.currentValue && QgsQuick.Utils.fileExists(homePath + "/" + image.currentValue)) {
          homePath + "/" + image.currentValue
      }
      else
        QgsQuick.Utils.getThemeIcon("ic_photo_notavailable_white_48dp")
    }
  }

  Button {
    id: button
    width: 36 * QgsQuick.Style.dp
    height: 36 * QgsQuick.Style.dp

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    onClicked: {
        photoCapturePanel.visible = true
        photoCapturePanel.targetDir = homePath
    }

    background: Image {
        source: QgsQuick.Utils.getThemeIcon("ic_camera_alt_border_24dp")
        width: button.width
        height: button.height
    }
  }

  Connections {
    target: photoCapturePanel
    onVisibleChanged    : {
        if (!photoCapturePanel.visible && photoCapturePanel.lastPhotoName !== "") {
            image.source = homePath + "/" + photoCapturePanel.lastPhotoName
            valueChanged(photoCapturePanel.lastPhotoName, false)
        }
    }
  }
}
