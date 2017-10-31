#ifndef QGSQUICKPROJECT_H
#define QGSQUICKPROJECT_H

#include <QObject>

class QgsMapLayer;

class QgsQuickProject : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString projectFile READ projectFile WRITE setProjectFile NOTIFY projectFileChanged)
  Q_PROPERTY(QList< QgsMapLayer* > layers READ layers NOTIFY projectFileChanged)
public:
  static QgsQuickProject* instance();

  void setProjectFile(const QString& filename);
  QString projectFile() const { return mFilename; }

  QList< QgsMapLayer* > layers() const;

signals:
  void projectFileChanged();

public slots:

protected:
  explicit QgsQuickProject(QObject *parent = 0);

  static QgsQuickProject* sInstance;

  QString mFilename;
};

#endif // QGSQUICKPROJECT_H
