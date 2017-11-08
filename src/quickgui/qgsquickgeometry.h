#ifndef QGSQUICKGEOMETRY_H
#define QGSQUICKGEOMETRY_H

#include "qgis_quick.h"
#include <qgsgeometry.h>
#include <qgsquickrubberbandmodel.h>

#include <QtPositioning/QGeoCoordinate>

class QUICK_EXPORT QgsQuickGeometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickRubberbandModel* rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( QgsVectorLayer* vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )

  public:
    explicit QgsQuickGeometry( QObject* parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    QgsQuickRubberbandModel* rubberbandModel() const;
    void setRubberbandModel( QgsQuickRubberbandModel* rubberbandModel );

    Q_INVOKABLE void applyRubberband();

    QgsVectorLayer* vectorLayer() const;
    void setVectorLayer( QgsVectorLayer* vectorLayer );

  signals:
    void rubberbandModelChanged();
    void vectorLayerChanged();

  private:
    QgsQuickRubberbandModel* mRubberbandModel;
    QgsVectorLayer* mVectorLayer;
};

#endif // QGSQUICKGEOMETRY_H
