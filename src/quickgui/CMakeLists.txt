SET(QGIS_QUICK_GUI_MOC_HDRS
  qgsquickproject.h
  qgsquickmapcanvasmap.h
  qgsquickplugin.h
  qgsquickmapsettings.h
)

SET(QGIS_QUICK_GUI_SRC
  qgsquickproject.cpp
  qgsquickplugin.cpp
  qgsquickmapcanvasmap.cpp
  qgsquickmapsettings.cpp
)

SET(QGIS_QUICK_GUI_QMLS
  qmldir
  qgsquickmapcanvas.qml
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
SET(QUICK_GUI_RUNTIME_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QgisQuick)
FOREACH (QML_FILE ${QGIS_QUICK_GUI_QMLS})
  GET_FILENAME_COMPONENT(QML_FILENAME ${QML_FILE} NAME)
  CONFIGURE_FILE(${QML_FILE} ${QUICK_GUI_RUNTIME_DIR}/${QML_FILENAME} COPYONLY)
ENDFOREACH(QML_FILE)

ADD_LIBRARY(qgis_quick MODULE ${QGIS_QUICK_GUI_SRC} ${QGIS_QUICK_GUI_MOC_HDRS} ${QGIS_QUICK_GUI_MOC_SRCS} ${QGIS_QUICK_GUI_QMLS})
TARGET_LINK_LIBRARIES(qgis_quick Qt5::Quick Qt5::Qml qgis_core Qt5::Xml Qt5::Widgets Qt5::Concurrent Qt5::Positioning)
SET_TARGET_PROPERTIES(qgis_quick PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${QUICK_GUI_RUNTIME_DIR})
ADD_DEFINITIONS(-DCORE_EXPORT=)

# QML Types Info (QtCreator QML, only usefull for development, so skip for android)
IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Android")
  SET(QGIS_QUICK_QUI_TYPEINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/QgisQuick/QgisQuick.qmltypes)
  FIND_PROGRAM(qmlplugindump_exe qmlplugindump)
  IF(NOT qmlplugindump_exe)
    MESSAGE(FATAL_ERROR "Could not locate qmlplugindump.")
  ELSE (NOT qmlplugindump_exe)
    MESSAGE(STATUS "qmlplugindump: ${qmlplugindump_exe}")
    ADD_CUSTOM_COMMAND(
        TARGET qgis_quick
        COMMAND ${qmlplugindump_exe}
        ARGS QgisQuick 1.0 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} --output ${QGIS_QUICK_QUI_TYPEINFO}
        DEPENDS ${QGIS_QUICK_GUI_QMLS}
        POST_BUILD
    )
  ENDIF(NOT qmlplugindump_exe)
  SET(QML_IMPORT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" CACHE STRING "Qt Creator extra QML import paths")
ENDIF(IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Android"))

# QML plugin installation
SET(QUICK_GUI_INSTALL_DIR ${QGIS_QML_DIR}/QgisQuick)
INSTALL(TARGETS qgis_quick
  RUNTIME DESTINATION ${QUICK_GUI_INSTALL_DIR}
  LIBRARY DESTINATION ${QUICK_GUI_INSTALL_DIR}
)
INSTALL(FILES ${QGIS_QUICK_GUI_QMLS} ${QGIS_QUICK_QUI_TYPEINFO}
  DESTINATION ${QUICK_GUI_INSTALL_DIR}
)
