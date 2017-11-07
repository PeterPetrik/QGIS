############################################################
# sources

SET(QGIS_QUICK_PLUGIN_MOC_HDRS
  qgsquickplugin.h
)

SET(QGIS_QUICK_PLUGIN_SRC
  qgsquickplugin.cpp
)

SET(QGIS_QUICK_PLUGIN_QMLS
  qml/qmldir
  qml/qgsquickmapcanvas.qml
  qml/qgsquickscalebar.qml
  qml/qgsquickfeatureform.qml
  qml/editor/qgsquickcheckbox.qml
  qml/editor/qgsquickdatetime.qml
  qml/editor/qgsquickexternalresource.qml
  qml/editor/qgsquickrelationreference.qml
  qml/editor/qgsquicktextedit.qml
  qml/editor/qgsquickvaluemap.qml
)

SET(QGIS_QUICK_GUI_MOC_HDRS
  qgsquickproject.h
  qgsquickmapcanvasmap.h
  qgsquickmapsettings.h
  qgsquickscalebarkit.h
  qgsquickidentifykit.h
)

SET(QGIS_QUICK_GUI_HDRS
  qgis_quick.h
)

SET(QGIS_QUICK_GUI_SRC
  qgsquickproject.cpp
  qgsquickmapcanvasmap.cpp
  qgsquickmapsettings.cpp
  qgsquickscalebarkit.cpp
  qgsquickidentifykit.cpp
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
ADD_DEFINITIONS(-DCORE_EXPORT=)

############################################################
# qgis_quick shared library
QT5_WRAP_CPP(QGIS_QUICK_GUI_MOC_SRCS ${QGIS_QUICK_GUI_MOC_HDRS})
IF(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_QUICK_GUI_MOC_SRCS} PROPERTIES COMPILE_FLAGS "/wd4512 /wd4996" )
ELSE(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_QUICK_GUI_MOC_SRCS} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations" )
ENDIF(MSVC)

ADD_LIBRARY(qgis_quick SHARED
    ${QGIS_QUICK_GUI_SRC}
    ${QGIS_QUICK_GUI_MOC_HDRS}
    ${QGIS_QUICK_GUI_MOC_SRCS}
    ${QGIS_QUICK_GUI_HDRS})
TARGET_LINK_LIBRARIES(qgis_quick Qt5::Quick Qt5::Qml Qt5::Xml Qt5::Widgets Qt5::Concurrent Qt5::Positioning qgis_core)

# Installation
INSTALL(TARGETS qgis_quick
  RUNTIME DESTINATION ${QGIS_BIN_DIR}
  LIBRARY DESTINATION ${QGIS_LIB_DIR}
  ARCHIVE DESTINATION ${QGIS_LIB_DIR}
  FRAMEWORK DESTINATION ${QGIS_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QGIS_INCLUDE_DIR})

############################################################
# qgis_quick_plugin module (QML) library

QT5_WRAP_CPP(QGIS_QUICK_PLUGIN_MOC_SRCS ${QGIS_QUICK_PLUGIN_MOC_HDRS})
IF(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_QUICK_PLUGIN_MOC_SRCS} PROPERTIES COMPILE_FLAGS "/wd4512 /wd4996" )
ELSE(MSVC)
  SET_SOURCE_FILES_PROPERTIES(${QGIS_QUICK_PLUGIN_MOC_SRCS} PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations" )
ENDIF(MSVC)

SET(QGIS_QUICK_PLUGIN_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QgisQuick)
FOREACH (QML_FILE ${QGIS_QUICK_PLUGIN_QMLS})
  GET_FILENAME_COMPONENT(QML_FILENAME ${QML_FILE} NAME)
  CONFIGURE_FILE(${QML_FILE} ${QGIS_QUICK_PLUGIN_RUNTIME_DIR}/${QML_FILENAME} COPYONLY)
ENDFOREACH(QML_FILE)

ADD_LIBRARY(qgis_quick_plugin MODULE
    ${QGIS_QUICK_PLUGIN_SRC}
    ${QGIS_QUICK_PLUGIN_MOC_HDRS}
    ${QGIS_QUICK_PLUGIN_MOC_SRCS}
    ${QGIS_QUICK_PLUGIN_QMLS}
)
TARGET_LINK_LIBRARIES(qgis_quick_plugin qgis_quick)
SET_TARGET_PROPERTIES(qgis_quick_plugin PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${QGIS_QUICK_PLUGIN_RUNTIME_DIR})
TARGET_COMPILE_DEFINITIONS(qgis_quick_plugin PRIVATE "-DQUICK_EXPORT=")

IF(QMLPLUGINDUMP_FOUND)
  # QML Types Info (QtCreator QML, only usefull for development)
  SET(QGIS_QUICK_QUI_TYPEINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QgisQuick/qgisquick.qmltypes)
  ADD_CUSTOM_COMMAND(
      TARGET qgis_quick_plugin
      COMMAND ${QMLPLUGINDUMP_EXECUTABLE}
      ARGS QgisQuick 1.0 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} --output ${QGIS_QUICK_QUI_TYPEINFO} --noinstantiate
      DEPENDS ${QGIS_QUICK_PLUGIN_QMLS}
      WORKING_DIRECTORY "${QMLPLUGINDUMP_WORK_DIR}"
      POST_BUILD
  )
ENDIF(QMLPLUGINDUMP_FOUND)

# Installation
SET(QUICK_GUI_INSTALL_DIR ${QGIS_QML_DIR}/QgisQuick)
INSTALL(TARGETS qgis_quick_plugin
  RUNTIME DESTINATION ${QUICK_GUI_INSTALL_DIR}
  LIBRARY DESTINATION ${QUICK_GUI_INSTALL_DIR}
)
INSTALL(FILES ${QGIS_QUICK_GUI_QMLS} ${QGIS_QUICK_QUI_TYPEINFO}
  DESTINATION ${QUICK_GUI_INSTALL_DIR}
)
