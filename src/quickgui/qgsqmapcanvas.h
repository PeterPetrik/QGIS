#ifndef QGSQMAPCANVAS_H
#define QGSQMAPCANVAS_H

#include <QQuickItem>

class QgsQMapCanvas : public QQuickItem
{
  Q_OBJECT
  Q_DISABLE_COPY(QgsQMapCanvas)

public:
  QgsQMapCanvas(QQuickItem *parent = 0);
  ~QgsQMapCanvas();
};

#endif // QGSQMAPCANVAS_H

