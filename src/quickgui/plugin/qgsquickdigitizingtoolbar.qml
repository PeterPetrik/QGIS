/***************************************************************************
 qgsquickdigitizingtoolbar.qml
  --------------------------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.6

//import org.qgis 1.0
//import "js/style.js" as Style
import QgisQuick 1.0 as QgsQuick

Row {
  id: digitizingToolbar
  property QgsQuick.RubberbandModel rubberbandModel
  property bool isDigitizing: rubberbandModel.vertexCount > 1 //!< Readonly
  visible: rubberbandModel.vectorLayer !== null

  spacing: 4 * dp
  padding: 4 * dp

  signal vertexAdded
  signal vertexRemoved
  signal cancel
  signal confirm

  Button {
    id: removeVertexButton
    iconSource: Style.getThemeIcon( "ic_remove_white_24dp" )
    visible: rubberbandModel.vertexCount > 1
    round: true
    bgcolor: "#616161"

    onClicked: {
      vertexRemoved()
    }
  }

  Button {
    id: cancelButton
    iconSource: Style.getThemeIcon( "ic_clear_white_24dp" )
    visible: rubberbandModel.vertexCount > 1
    round: true
    bgcolor: "#616161"

    onClicked: {
      cancel()
    }
  }

  Button {
    id: confirmButton
    iconSource: {
      if ( Number( rubberbandModel.geometryType ) === 0 )
      {
        Style.getThemeIcon( "ic_create_white_24dp" )
      }
      else
      {
        Style.getThemeIcon( "ic_save_white_24dp" )
      }
    }
    visible: {
      if ( Number( rubberbandModel.geometryType ) === 0 )
      {
        true
      }
      else if  ( Number( rubberbandModel.geometryType ) === 1 )
      {
        // Line: at least 2 points
        rubberbandModel.vertexCount > 1
      }
      else if  ( Number( rubberbandModel.geometryType ) === 2 )
      {
        // Polygon: at least 3 points
        rubberbandModel.vertexCount > 2
      }
    }
    round: true
    bgcolor: "#FFD600"

    onClicked: {
      confirm()
    }
  }

  Button {
    id: addVertexButton
    iconSource: {
      if( digitizingToolbar.rubberbandModel.vertexCount === 1 )
        Style.getThemeIcon( "ic_create_white_24dp" )
      else
        Style.getThemeIcon( "ic_add_white_24dp" )
    }
    // Not for points
    visible: Number(rubberbandModel.geometryType) === 1 || Number(rubberbandModel.geometryType) === 2
    round: true
    bgcolor: "#2E7D32"

    onClicked: {
      vertexAdded()
    }
  }
}
