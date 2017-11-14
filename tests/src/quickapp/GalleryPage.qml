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

        /* Scale Bar */
        Text {text: qsTr( "ScaleBar")}
        QgsQuick.ScaleBar {}

        /* Text Edit */
        Text {text: qsTr( "TextEdit")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.TextEdit {
                    property string value: "Hello Text Edit!"
                    property var config: {"UseHtml": false, "IsMultiline": false}
                    property var field: {"isNumeric": false}
                }
                //source: './qgsquicktextedit.qml'
            }
        }

        /* Value Map */
        Text {text: qsTr( "ValueMap")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.ValueMap {
                    property string value: "a"
                    property var config: {"map": {"A": "a", "B": "b", "C": "c"}}
                }
            }
        }

        /* Check Box */
        Text {text: qsTr( "CheckBox")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.CheckBox {
                    property string value: "yes"
                    property var config: {"CheckedState": "yes", "UncheckedState": "no"}
                }
            }
        }

        /* Date Time */
        Text {text: qsTr( "DateTime")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.DateTime {
                    property var value: null
                    property var config: {"allow_null": true, "display_format": "d MMM yyyy h:m:s"}
                }
            }
        }

        /* External Resource (Picture) */
        Text {text: qsTr( "ExternalResource")}
        Item {
            width: 200; height: 50
            Loader {
                anchors.fill: parent
                sourceComponent: QgsQuick.ExternalResource {
                    property Image image
                    property string homePath: ""
                    property var value: "a.png"
                    property var config
                }
            }
        }
    }
}
