#ifndef QGSQUICKMAPCANVAS_H
#define QGSQUICKMAPCANVAS_H

#include <QQuickItem>

class QgsQuickMapCanvas : public QQuickItem
{
  Q_OBJECT
  Q_DISABLE_COPY(QgsQuickMapCanvas)

public:
  QgsQuickMapCanvas(QQuickItem *parent = 0);
  ~QgsQuickMapCanvas();
};

#endif // QGSQUICKMAPCANVAS_H

