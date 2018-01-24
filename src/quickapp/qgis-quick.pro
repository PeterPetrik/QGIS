TEMPLATE = app

!android {
  error(Use CMake build system for non-android builds)
}

include(config.pri)
OSGEO4A_STAGE_DIR = $${OSGEO4A_DIR}/stage 
OSGEO4A_QML_DIR = $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH/qml
OSGEO4A_LIB_DIR = $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/
QT_LIBS_DIR = $$dirname(QMAKE_QMAKE)/../lib
exists($${OSGEO4A_LIB_DIR}/libqgis_core.so) {
  message("Building ANDROID APK with $${OSGEO4A_STAGE_DIR}")
} else {
  error("Missing QGIS Core library in $${OSGEO4A_LIB_DIR}")
}

INCLUDEPATH += \
  $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/include/qgis \
  $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/include

LIBS += -L$${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib
INCLUDEPATH += $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/include
LIBS += -lqgis_core

DEFINES += "CORE_EXPORT="

QT += qml quick widgets xml concurrent positioning quickcontrols2
QT += network svg printsupport sql
QT += script opengl
  

SOURCES += main.cpp
HEADERS +=

RESOURCES += qml.qrc
# i18n.qrc \
# img.qrc

TRANSLATIONS +=
# i18n/manioc_sk.ts \
# i18n/manioc_es.ts

lupdate_only {
    SOURCES += *.qml
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $${OSGEO4A_QML_DIR}

QMAKE_CXXFLAGS += -std=c++11

# files from this folder will be added to the package
# (and will override any default files from Qt - template is in $QTDIR/src/android)
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

# this makes the manifest visible from Qt Creator
DISTFILES += android/AndroidManifest.xml

# packaging
ANDROID_EXTRA_LIBS += \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libcrystax.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libexpat.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgeos.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgeos_c.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgslcblas.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libsqlite3.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libcharset.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libiconv.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libfreexl.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libtiff.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgdal.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libproj.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialindex.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libpq.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialite.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqca-qt5.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqgis_core.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqgis_native.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libqt5keychain.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libzip.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialiteprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libdelimitedtextprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgdalprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libgpxprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libmssqlprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libogrprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libowsprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libpostgresprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libspatialiteprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libwcsprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libwfsprovider.so \
    $${OSGEO4A_STAGE_DIR}/$$ANDROID_TARGET_ARCH$$/lib/libwmsprovider.so \
    $$QT_LIBS_DIR/libQt5OpenGL.so \
    $$QT_LIBS_DIR/libQt5PrintSupport.so \
    $$QT_LIBS_DIR/libQt5Sensors.so \
    $$QT_LIBS_DIR/libQt5Sql.so \
    $$QT_LIBS_DIR/libQt5Script.so \
    $$QT_LIBS_DIR/libQt5Svg.so \
    $$QT_LIBS_DIR/libQt5AndroidExtras.so

ANDROID_EXTRA_PLUGINS += $${OSGEO4A_QML_DIR}
