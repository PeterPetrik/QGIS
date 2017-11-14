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
import QtQuick.Window 2.2
import QgisQuick 1.0 as QgsQuick
import '.' as QgsQuickApp

Window {
    visible: true

    width: 800
    height: 600

    Item {
      id: stateMachine

      states: [
        State {
          name: "browse"
        },

        State {
          name: "digitize"
        }
      ]
      state: "browse"
    }

    QgsQuick.Project {
        id: project
        fileName: qgisProject
    }

    QgsQuick.MapCanvas {
        id: mapCanvas

        property var vectorLayer: layerTreeView.currentLayer

        mapSettings.project: project
        mapSettings.layers: project.layers
        width: parent.width
        height: parent.height

        onClicked: {
            var feature = identifyKit.identifyOne( vectorLayer, Qt.point( mouse.x, mouse.y ) )
            if (feature.isValid)
            {
                overlayFeatureForm.visible = false;
            } else {
                overlayFeatureForm.visible = true;
                overlayFeatureForm.state = "Edit";
                digitizingFeature.feature = feature;

            }
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
        visible: true
        project: project

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20 // Sets all margins at once
    }

    QgsQuick.Rubberband {
      id: digitizingRubberband
      width: 2 * dp

      mapSettings: mapCanvas.mapSettings

      model: QgsQuick.RubberbandModel {
        currentCoordinate: coordinateLocator.currentCoordinate
        vectorLayer: layerTreeView.currentLayer
        crs: mapCanvas.mapSettings.destinationCrs
      }

      anchors.fill: parent

      visible: stateMachine.state === "digitize"
    }


    QgsQuick.FeatureModel {
      id: digitizingFeature
      currentLayer: layerTreeView.currentLayer

      geometry: QgsQuick.Geometry {
        id: digitizingGeometry
        rubberbandModel: digitizingRubberband.model
        vectorLayer: layerTreeView.currentLayer
      }
    }

    QgsQuick.FeatureForm {
      id: overlayFeatureForm

      anchors { right: parent.right; top: parent.top; bottom: parent.bottom }

      //width: qfieldSettings.fullScreenIdentifyView ? parent.width : parent.width / 3
      width: parent.width/ 3


      model: QgsQuick.AttributeFormModel {
        featureModel: digitizingFeature
      }


      state: "Add"

      visible: false

      onSaved: {
        visible = false


        if ( state === "Add" )
          digitizingRubberband.model.reset()

      }
      onCancelled: visible = false
    }

}
