import QtQuick 2.5
import QtQuick.Controls 2.0
import QgisQuick 1.0 as QgsQuick


Item {
  signal valueChanged(var value, bool isNull)

  anchors.left: parent.left
  anchors.right: parent.right

  height: Math.max(image.height, button.height)

  property QgsQuick.PictureSource __pictureSource


  Image {
    property var currentValue: value

    id: image
    width: 200 * QgsQuick.Style.dp
    autoTransform: true
    fillMode: Image.PreserveAspectFit

    source: {
      if (image.status === Image.Error)
        QgsQuick.Utils.getThemeIcon("ic_broken_image_black_24dp")
      else if (image.currentValue)
        'file://' + homePath + '/' + currentValue
      else
        QgsQuick.Utils.getThemeIcon("ic_photo_notavailable_white_48dp")
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (image.currentValue)
          QgsQuick.Utils.open(image.source, "image/*");
      }
    }
  }

  Button {
    id: button
    width: 36 * QgsQuick.Style.dp
    height: 36 * QgsQuick.Style.dp

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    onClicked: __pictureSource = QgsQuick.Utils.getPicture(homePath + '/DCIM')

    background: Image {
        source: QgsQuick.Utils.getThemeIcon("ic_camera_alt_border_24dp")
        width: button.width
        height: button.height
    }
  }

  Connections {
    target: __pictureSource
    onPictureReceived: {
      valueChanged('DCIM/' + path, false)
    }
  }
}
