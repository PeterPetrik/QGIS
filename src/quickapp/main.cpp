#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QtDebug>

#ifdef ANDROID
#include <QFile>
#include <QDir>
#endif

#include "qgsapplication.h"

static void init_qgis()
{
#ifndef ANDROID
  if (::getenv("QGIS_PREFIX_PATH") == 0)
  {
    qDebug("== Must set QGIS_PREFIX_PATH in order to get QGIS Quick module running! ==");
    return;
  }
#endif

  QTime t;
  t.start();

#ifdef ANDROID
  QDir myDir( QDir::homePath() );
  myDir.cdUp();
  QString prefixPath = myDir.absolutePath();  // something like: /data/data/org.qgis.quick
  ::setenv("QGIS_PREFIX_PATH", prefixPath.toUtf8().constData(), true);
#endif

  // if not on Android, QGIS_PREFIX_PATH env variable should have been set already
  QgsApplication::init();
  QgsApplication::initQgis();

#ifdef ANDROID
  // QGIS plugins on Android are in the same path as other libraries
  QgsApplication::setPluginPath( QApplication::applicationDirPath() );

  QString qgisDataSubdir = "qgis-data";
  QDir homeDir = QDir::home();
  QString qgisDataPath = QDir::homePath() + "/" + qgisDataSubdir;
  ::setenv("QGIS_QUICK_DATA_PATH", qgisDataPath.toUtf8().constData(), true);

  QString assetsBasePath( "assets:" );
  qDebug("assets base path:  %s", assetsBasePath.toLatin1().data());

  // make sure all necessary QGIS data are available - unpacked from assets
  //if (!homeDir.exists(qgisDataSubdir)) // do it always on startup - for now
  {
    bool res = homeDir.mkpath(qgisDataSubdir);
    qDebug("mkpath [%d] %s", res, qgisDataPath.toLatin1().data());
    QDir qgisDataDir(qgisDataPath);

    QStringList qgisDataFiles;
    qgisDataFiles << "resources/qgis.db" << "resources/srs.db";
    qgisDataFiles << "bg.tif";
    qgisDataFiles << "lines.shp" <<"lines.dbf" << "lines.prj" << "lines.shx";
    qgisDataFiles << "points.shp" <<"points.dbf" << "points.prj" << "points.shx";
    qgisDataFiles << "polys.shp" <<"polys.dbf" << "polys.prj" << "polys.shx";
    qgisDataFiles << "test_project.qgs";

    foreach (const QString& dataFile, qgisDataFiles)
    {
      QFile f(assetsBasePath + "/" + qgisDataSubdir + "/" + dataFile);
      QString destFilePath = qgisDataPath + "/" + dataFile;
      QDir destFileDir = QFileInfo(destFilePath).absoluteDir();
      if (!destFileDir.exists())
      {
        bool mkres = qgisDataDir.mkpath(QFileInfo(dataFile).dir().path());
        qDebug("mkpath [%d] %s", mkres, destFileDir.absolutePath().toLatin1().data());
      }
      if (!QFile(destFilePath).exists())
      {
        bool res = f.copy(destFilePath);
        qDebug("copying [%d] %s", res, destFilePath.toLatin1().data());
        // by default the permissions are set as readable for everyone - and not writable!
        res = QFile::setPermissions(destFilePath, QFile::ReadUser|QFile::WriteUser|QFile::ReadOwner|QFile::WriteOwner);
        qDebug("chmod [%d]", res);
      }
    }
  }

  QgsApplication::setPkgDataPath(qgisDataPath);
#else
  ::setenv("QGIS_QUICK_DATA_PATH", QGIS_QUICK_DATA_PATH, true);
#endif

  // make sure the DB exists - otherwise custom projections will be failing
  if (!QgsApplication::createDatabase())
    qDebug("Can't create qgis user DB!!!");

  qDebug("qgis_init %f [s]", t.elapsed()/1000.0);
}

int main(int argc, char *argv[])
{
  QgsApplication app(argc, argv, true);

#ifdef QGIS_PREFIX_PATH
  ::setenv("QGIS_PREFIX_PATH", QGIS_PREFIX_PATH, true);
#endif
  init_qgis();
  qDebug("%s", QgsApplication::showSettings().toLocal8Bit().data());
  QString dataDir(::getenv("QGIS_QUICK_DATA_PATH"));
  QString projectFile = dataDir + "/test_project.qgs";

  QQmlEngine engine;
  QQmlComponent component(&engine, QUrl("qrc:/main.qml"));
  QObject *object = component.create();

  qDebug() << "project file: " << projectFile;
  object->setProperty("projectFile", projectFile);

  //QQmlApplicationEngine engine;
  //engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}

