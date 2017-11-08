/***************************************************************************
  main.qml
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
import '.'

Window {
    id: mainWindow
    visible: true

    minimumWidth: 800
    minimumHeight: 600

    TabBar {
        id: bar
        width: parent.width

        TabButton {
            text: qsTr("Map")
        }

        TabButton {
            text: qsTr("Gallery")
        }
        TabButton {
            text: qsTr("About")
        }
    }

    StackLayout {
        id: stack
        width: parent.width
        anchors.top: bar.bottom

        currentIndex: bar.currentIndex

        MapPage
        {
            id: mapTab

            width: mainWindow.width
            height: mainWindow.height - bar.height
        }

        GalleryPage {
            id: galleryTab
        }

        AboutPage {
            id: aboutTab
        }
    }
}
