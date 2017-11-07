/***************************************************************************
  MapPage.qml
  --------------------------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2
import QgisQuick 1.0 as QgsQuick
import '.' as QgsQuickApp

Item {
    visible: true

    width: 800 //TODO how to take from parent?
    height: 700 //TODO how to take from parent?

    QgsQuick.Project {
        id: project
        fileName: qgisProject
    }
    QgsQuick.MapCanvas {
        id: mapCanvas
        mapSettings.project: project
        mapSettings.layers: project.layers
        width: parent.width
        height: parent.height

        onClicked: {
            identifyKit.identify( Qt.point( mouse.x, mouse.y ) )
        }
    }

    ColumnLayout {
        id: mainMenu
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20 // Sets all margins at once
        states: [
            State {
                name: "" // Default state
                PropertyChanges { target: about; visible: false}
                PropertyChanges { target: layerTreeView; visible: false}
            },
            State {
                name: "LAYERS";
                PropertyChanges { target: layerTreeView; visible: true}
            },
            State {
                name: "IDENTIFY";
                PropertyChanges { target: layerTreeView; visible: false}
            }
        ]

        Button {
            id: layersButton
            text: "Layers"
            onClicked: parent.state = "LAYERS"
        }

        Button {
            id: identifyButton
            text: "Identify"
            onClicked: parent.state = "IDENTIFY"
        }
    }

    QgsQuick.IdentifyKit {
        id: identifyKit
        mapSettings: mapCanvas.mapSettings
    }

    QgsQuick.ScaleBar {
        id: scaleBar
        preferredWidth: 300 * dp
        mapSettings: mapCanvas.mapSettings

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }

    QgsQuickApp.LayerTreeView {
        id: layerTreeView
        visible: false
        project: project

        anchors.top: parent.top
        anchors.left: mainMenu.right
        anchors.margins: 20 // Sets all margins at once
    }

    /*
    QgsQuick.FeatureForm {
      id: overlayFeatureForm

      anchors { right: parent.right; top: parent.top; bottom: parent.bottom }

      //width: qfieldSettings.fullScreenIdentifyView ? parent.width : parent.width / 3
      width: parent.width


      //model: AttributeFormModel {
      //  featureModel: digitizingFeature
      //}


      state: "Add"

      visible: false

      onSaved: {
        visible = false


        //if ( state === "Add" )
        //  digitizingRubberband.model.reset()
        //
      }
      onCancelled: visible = false
    }
    */
}
