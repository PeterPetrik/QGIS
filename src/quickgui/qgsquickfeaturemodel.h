/***************************************************************************
 qgsquickfeaturemodel.h
  --------------------------------------
  Date                 : 10.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKFEATUREMODEL_H
#define QGSQUICKFEATUREMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "qgsfeature.h"
#include "qgsvectorlayer.h"

#include "qgis_quick.h"
#include "qgsquickfeaturelayerpair.h"

/**
 * \ingroup quick
 * Item model implementation for attributes of QgsFeature: each attribute gets a row in the model.
 *
 * \note QML Type: FeatureModel
 *
 * \since QGIS 3.4
 */
class QUICK_EXPORT QgsQuickFeatureModel : public QAbstractListModel
{
    Q_OBJECT

    /**
     * QgsQuickFeature in the model.
     */
    Q_PROPERTY( QgsQuickFeatureLayerPair feature READ feature WRITE setFeature NOTIFY featureChanged )

    /**
     * Feature roles enum.
     */
    Q_ENUMS( FeatureRoles )

  public:
    enum FeatureRoles
    {
      AttributeName = Qt::UserRole + 1,  //!< Attribute's display name (the original field name or a custom alias)
      AttributeValue,                    //!< Value of the feature's attribute
      Field,                             //!< Field definition (QgsField)
      RememberAttribute                  //!< Remember attribute value for next feature
    };

    //! Creates a new feature model
    explicit QgsQuickFeatureModel( QObject *parent = 0 );

    /**
     * Creates a new feature model
     * \param feat Feature set to model,
     * \param parent Parent object.
     */
    explicit QgsQuickFeatureModel( const QgsFeature &feat, QObject *parent = 0 );

    //! List of all role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Equals to number of feature attributes.
    int rowCount( const QModelIndex &parent ) const override;

    /**
     * Returns model data according params.
     * \param index Index in the model
     * \param role Feature role.
     */
    QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Sets data to the model according params.
     * \param index Index in the model
     * \param value QVariant value
     * \param role Feature role.
     */
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    /**
     * Commits the edit buffer of this layer.
     * May change in the future to only commit the changes buffered in this model.
     *
     * @return Success of the operation.
     */
    Q_INVOKABLE bool save();

    /**
     * Deletes the current feature from the layer and commit the changes.
     * @return Success of the operation.
     */
    Q_INVOKABLE bool deleteFeature();

    /**
     * Resets the feature to the original values and dismiss any buffered edits.
     */
    Q_INVOKABLE void reset();

    //! Adds mFeature to mLayer
    Q_INVOKABLE void create();

    /**
     * Suppress layer's QgsEditFormConfig
     *
     * \sa QgsEditFormConfig::suppress
     */
    Q_INVOKABLE bool suppressFeatureForm() const;

    //! Resets remembered attributes
    Q_INVOKABLE virtual void resetAttributes();

    //! Gets remembered attributes
    QVector<bool> rememberedAttributes() const;

    //!\copydoc QgsQuickFeatureModel::feature
    QgsQuickFeatureLayerPair feature() const;

    //!\copydoc QgsQuickFeatureModel::feature
    void setFeature( const QgsQuickFeatureLayerPair &feature );

  public slots:

  signals:
    //! Feature changed
    void featureChanged();
    void layerChanged();

  protected:
    //! Commits model changes
    bool commit();
    //! Starts editing model
    bool startEditing();

    QgsQuickFeatureLayerPair mFeature;
    QVector<bool> mRememberedAttributes;
  private:
    void setFeatureOnly( const QgsFeature &feature );
    void setLayer( QgsVectorLayer *layer );
};

#endif // QGSQUICKFEATUREMODEL_H
