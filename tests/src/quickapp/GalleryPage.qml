/***************************************************************************
  EditorGalleryPage.qml
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
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

import QgisQuick 1.0 as QgsQuick

Item {
    Grid {
        columns: 2
        spacing: 25

        Text {text: qsTr( "ScaleBar")}
        QgsQuick.ScaleBar {}


        Text {text: qsTr( "TextEdit")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.TextEdit {
                    property var value: "Hello Text Edit!"
                    property var config: {"UseHtml": false, "IsMultiline": false}
                    property var field: {"isNumeric": false}
                }
                //source: './qgsquicktextedit.qml'
            }
        }

        Text {text: qsTr( "ValueMap")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.ValueMap {
                    property var value: "a"
                    property var config: {"map": {"A": "a", "B": "b", "C": "c"}}
                }
            }
        }
    }
}
