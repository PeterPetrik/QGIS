#ifndef QGSQUICKQGSSGRUBBERBAND_H
#define QGSQUICKQGSSGRUBBERBAND_H

#include "qgis_quick.h"

#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>

#include <qgspoint.h>
#include <qgswkbtypes.h>


class QgsQuickRubberbandModel;

/**
 * This is used to render a rubberband on the scene graph.
 *
 * This cannot be considered stable API.
 */

class QUICK_NO_EXPORT QgsQuickSGRubberband : public QSGNode
{
  public:
    QgsQuickSGRubberband( const QVector<QgsPoint>& points, QgsWkbTypes::GeometryType type, const QColor& color, qreal width );

  private:
    QSGGeometryNode* createLineGeometry( const QVector<QgsPoint>& points, qreal width );
    QSGGeometryNode* createPolygonGeometry( const QVector<QgsPoint>& points );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSQUICKQGSSGRUBBERBAND_H
