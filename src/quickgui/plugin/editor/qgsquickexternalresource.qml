import QtQuick 2.5
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
      else if (currentValue)
        'file://' + homePath + '/' + currentValue
      else
        QgsQuick.Utils.getThemeIcon("ic_photo_notavailable_white_48dp")
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        if (currentValue)
          QgsQuick.Utils.open(image.source, "image/*");
      }
    }
  }

  QgsQuick.Button {
    id: button
    width: 36 * QgsQuick.Style.dp
    height: 36 * QgsQuick.Style.dp

    anchors.right: parent.right
    anchors.bottom: parent.bottom

    bgcolor: "transparent"

    onClicked: __pictureSource = QgsQuick.Utils.getPicture(homePath + '/DCIM')

    iconSource: QgsQuick.Utils.getThemeIcon("ic_camera_alt_border_24dp")
  }

  Connections {
    target: __pictureSource
    onPictureReceived: {
      valueChanged('DCIM/' + path, false)
    }
  }
}
