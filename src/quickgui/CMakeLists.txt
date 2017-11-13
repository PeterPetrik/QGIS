############################################################
# sources
SET(QGIS_QUICK_GUI_MOC_HDRS
  qgsquickproject.h
  qgsquickmapcanvasmap.h
  qgsquickmapsettings.h
  qgsquickscalebarkit.h
  qgsquickidentifykit.h
  qgsquickattributeformmodel.h
  qgsquickattributeformmodelbase.h
  qgsquickfeaturemodel.h
  qgsquickgeometry.h
  qgsquickrubberband.h
  qgsquickrubberbandmodel.h
  qgsquicksubmodel.h
  qgsquickpicturesource.h
  qgsquickutils.h
)

SET(QGIS_QUICK_GUI_HDRS
  qgis_quick.h
  qgsquicksgrubberband.h
)

SET(QGIS_QUICK_GUI_SRC
  qgsquickproject.cpp
  qgsquickmapcanvasmap.cpp
  qgsquickmapsettings.cpp
  qgsquickscalebarkit.cpp
  qgsquickidentifykit.cpp
  qgsquickattributeformmodel.cpp
  qgsquickattributeformmodelbase.cpp
  qgsquickfeaturemodel.cpp
  qgsquickgeometry.cpp
  qgsquickrubberband.cpp
  qgsquickrubberbandmodel.cpp
  qgsquicksgrubberband.cpp
  qgsquicksubmodel.cpp
  qgsquickpicturesource.cpp
  qgsquickutils.cpp
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
TARGET_COMPILE_DEFINITIONS(qgis_quick PRIVATE "-DDQT_NO_FOREACH")

# Installation
INSTALL(TARGETS qgis_quick
  RUNTIME DESTINATION ${QGIS_BIN_DIR}
  LIBRARY DESTINATION ${QGIS_LIB_DIR}
  ARCHIVE DESTINATION ${QGIS_LIB_DIR}
  FRAMEWORK DESTINATION ${QGIS_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QGIS_INCLUDE_DIR})

############################################################
# qgis_quick_plugin module (QML) library
ADD_SUBDIRECTORY(plugin)

