SET(QGIS_QUICK_GUI_MOC_HDRS
  mapengine.h
  mapimage.h
  maplayer.h
  mapview.h
  plugin.h
  positionengine.h
  project.h
)

SET(QGIS_QUICK_GUI_SRC
  plugin.cpp
  mapengine.cpp
  mapimage.cpp
  maplayer.cpp
  mapview.cpp
  project.cpp
  positionengine.cpp
)

SET(QGIS_QUICK_GUI_QMLS
  qmldir
  MapCanvas.qml
  ScaleBar.qml
  VertexMarker.qml
  VertexMarkerBase.qml
  PositionMarker.qml
  PositionAccuracy.qml
)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}

  ${CMAKE_SOURCE_DIR}/src/core
  ${CMAKE_SOURCE_DIR}/src/core/annotations
  ${CMAKE_SOURCE_DIR}/src/core/auth
  ${CMAKE_SOURCE_DIR}/src/core/composer
  ${CMAKE_SOURCE_DIR}/src/core/fieldformatter
  ${CMAKE_SOURCE_DIR}/src/core/geometry
  ${CMAKE_SOURCE_DIR}/src/core/layertree
  ${CMAKE_SOURCE_DIR}/src/core/layout
  ${CMAKE_SOURCE_DIR}/src/core/locator
  ${CMAKE_SOURCE_DIR}/src/core/metadata
  ${CMAKE_SOURCE_DIR}/src/core/providers/memory
  ${CMAKE_SOURCE_DIR}/src/core/raster
  ${CMAKE_SOURCE_DIR}/src/core/scalebar
  ${CMAKE_SOURCE_DIR}/src/core/symbology
  ${CMAKE_SOURCE_DIR}/src/core/effects
  ${CMAKE_SOURCE_DIR}/src/core/metadata
  ${CMAKE_SOURCE_DIR}/src/core/expression

  ${CMAKE_BINARY_DIR}/src/core
)

INCLUDE_DIRECTORIES(SYSTEM
  ${LIBZIP_INCLUDE_DIRS}
  ${SPATIALINDEX_INCLUDE_DIR}
  ${PROJ_INCLUDE_DIR}
  ${GEOS_INCLUDE_DIR}
  ${GDAL_INCLUDE_DIR}
  ${EXPAT_INCLUDE_DIR}
  ${SQLITE3_INCLUDE_DIR}
  ${SPATIALITE_INCLUDE_DIR}
  ${QCA_INCLUDE_DIR}
  ${QTKEYCHAIN_INCLUDE_DIR}
)

QT5_WRAP_CPP(QGIS_QUICK_GUI_MOC_SRCS ${QGIS_QUICK_GUI_MOC_HDRS})

IF(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_CORE_MOC_SRCS} PROPERTIES COMPILE_FLAGS "/wd4512 /wd4996" )
ELSE(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_CORE_MOC_SRCS} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations" )
ENDIF(MSVC)

# QML plugin
SET(QUICK_GUI_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/qgis)
FOREACH (QML_FILE ${QGIS_QUICK_GUI_QMLS})
  GET_FILENAME_COMPONENT(QML_FILENAME ${QML_FILE} NAME)
  CONFIGURE_FILE(${QML_FILE} ${QUICK_GUI_RUNTIME_DIR}/${QML_FILENAME} COPYONLY)
ENDFOREACH(QML_FILE)

ADD_LIBRARY(qgis-quick-gui MODULE ${QGIS_QUICK_GUI_SRC} ${QGIS_QUICK_GUI_MOC_HDRS} ${QGIS_QUICK_GUI_MOC_SRCS} ${QGIS_QUICK_GUI_QMLS})
TARGET_LINK_LIBRARIES(qgis-quick-gui Qt5::Quick Qt5::Qml qgis_core Qt5::Xml Qt5::Widgets Qt5::Concurrent Qt5::Positioning)
SET_TARGET_PROPERTIES(qgis-quick-gui PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${QUICK_GUI_RUNTIME_DIR})
ADD_DEFINITIONS(-DCORE_EXPORT=)

# QML plugin installation
SET(QUICK_GUI_INSTALL_DIR ${QGIS_QML_DIR}/qgis)
INSTALL(TARGETS qgis-quick-gui
  RUNTIME DESTINATION ${QUICK_GUI_INSTALL_DIR}
  LIBRARY DESTINATION ${QUICK_GUI_INSTALL_DIR}
)
INSTALL(FILES ${QGIS_QUICK_GUI_QMLS}
  DESTINATION ${QUICK_GUI_INSTALL_DIR}
)

