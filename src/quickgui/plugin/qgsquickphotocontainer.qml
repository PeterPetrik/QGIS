/***************************************************************************
 qgsquickpositionmarker.qml
  --------------------------------------
  Date                 : Dec 2017
  Copyright            : (C) 2017 by Viktor Sklencar
  Email                : vsklencar at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.3
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2
import QtQml 2.2
import QtMultimedia 5.8
import QtGraphicalEffects 1.0
import QgisQuick 1.0 as QgsQuick

Drawer {
    // photo folder
    property var targetDir
    property var lastPhotoName
    property int iconSize: photoPanel.width/30

    id: photoPanel
    visible: false
    modal: true
    interactive: true
    dragMargin: 0 // prevents opening the drawer by dragging.

    background: Rectangle {
        color: "#7b909d"
        opacity: 0.7
    }

    onVisibleChanged: {
        if (visible) {
            camera.setCameraState(Camera.ActiveState)
            camera.start()
        } else {
            camera.stop()
            photoPreview.visible = false
        }
    }


    Column {
        //spacing: 20 * QgsQuick.Style.dp
        anchors.centerIn: parent

        Text {
            text: "Photo capture"
            color: "white"
            font.pixelSize: photoPanel.width / 20
            font.bold: true
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    // PhotoCapture item
    Item {

        property bool saveImage: false

        id: captureItem
        width: photoPanel.width
        height: photoPanel.height


        Component.onDestruction: {
            if (!captureItem && camera.imageCapture.capturedImagePath != ""){
                captureItem.saveImage = false
                QgsQuick.Utils.remove(camera.imageCapture.capturedImagePath)
            }
            captureItem.saveImage = false
        }

        Camera {
            id: camera
            cameraState: Camera.UnloadedState

            imageCapture {
                onImageCaptured: {
                    // Show the preview in an Image
                    photoPreview.source = preview
                }
            }
        }

        VideoOutput {
            source: camera
            focus : visible // to receive focus and capture key events when visible
            anchors.fill: parent

            Rectangle {
                id: captureBtn
                property int borderWidth: 10 * QgsQuick.Style.dp
                width: parent.width/20
                height: parent.width/20
                color: "white"
                border.color: "black"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                border.width: borderWidth
                radius: width*0.5
                antialiasing: true

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (targetDir !== "") {
                            camera.imageCapture.captureToLocation(photoPanel.targetDir);
                        } else {
                            camera.imageCapture.capture();
                            // TODO copy image to project file
                        }
                        photoPreview.visible = true;
                    }
                }

                Image {
                    id: captureBtnImage
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                    sourceSize.height: captureBtn.height/2
                    height: captureBtn.height/2
                    source: QgsQuick.Utils.getThemeIcon("ic_camera_alt_border_24dp")
                }

            }

            // Photo capture button
            //            Button {
            //                id: captureBtn
            //                text: ""
            //                //implicitWidth: height
            //                anchors.right: parent.right
            //                anchors.verticalCenter: parent.verticalCenter
            //                //Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            //                property string source: QgsQuick.Utils.getThemeIcon("ic_camera_alt_border_24dp")

            //                onClicked: {
            //                    if (targetDir !== "") {
            //                        camera.imageCapture.captureToLocation(photoPanel.targetDir);
            //                    } else {
            //                        camera.imageCapture.capture();
            //                        // TODO copy image to project file
            //                    }
            //                    photoPreview.visible = true;
            //                }

            //            }
        }

        Image {
            id: photoPreview
            width: window.width
            height: window.height

            //            Button {
            //                text: qsTr("OK")
            //                anchors.bottom: parent.bottom
            //                anchors.left: parent.left
            //                anchors.margins: 10
            //                visible: camera.imageCapture.capturedImagePath != ""

            //                onClicked: {
            //                    console.log(camera.imageCapture.capturedImagePath)
            //                    captureItem.saveImage = true
            //                    photoPanel.visible = false
            //                    photoPanel.lastPhotoName = QgsQuick.Utils.getFileName(camera.imageCapture.capturedImagePath)
            //                }
            //            }

            // Cancel button
            Image {
                width: photoPanel.iconSize
                height: photoPanel.iconSize
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 10
                visible: camera.imageCapture.capturedImagePath != ""
                source: QgsQuick.Utils.getThemeIcon("ic_clear_black_18dp")


                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        captureItem.saveImage = false
                        photoPreview.visible = false
                        if (camera.imageCapture.capturedImagePath != "") {
                            QgsQuick.Utils.remove(camera.imageCapture.capturedImagePath)
                        }
                    }

                }
            }

            // Ok button
            Image {
                width: photoPanel.iconSize
                height: photoPanel.iconSize
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10
                visible: camera.imageCapture.capturedImagePath != ""
                source: QgsQuick.Utils.getThemeIcon("ic_add_to_photos")

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(camera.imageCapture.capturedImagePath)
                        captureItem.saveImage = true
                        photoPanel.visible = false
                        photoPanel.lastPhotoName = QgsQuick.Utils.getFileName(camera.imageCapture.capturedImagePath)
                    }
                }
            }

            //            Button {
            //                text: qsTr("Cancel")
            //                anchors.bottom: parent.bottom
            //                anchors.right: parent.right
            //                anchors.margins: 10
            //                width: photoPanel.iconSize
            //                height: photoPanel.iconSize
            //                visible: camera.imageCapture.capturedImagePath != ""

            //                onClicked: {
            //                    captureItem.saveImage = false
            //                    photoPreview.visible = false
            //                    if (camera.imageCapture.capturedImagePath != "") {
            //                        QgsQuick.Utils.remove(camera.imageCapture.capturedImagePath)
            //                    }
            //                }

            //            }
        }
    }
}

