/***************************************************************************
 qgsquickfeaturemodel.cpp
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

#include "qgis.h"
#include "qgsmessagelog.h"
#include "qgsvectorlayer.h"

#include "qgsquickfeaturemodel.h"

QgsQuickFeatureModel::QgsQuickFeatureModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( this, &QgsQuickFeatureModel::modelReset, this, &QgsQuickFeatureModel::featureLayerPairChanged );
}

QgsQuickFeatureLayerPair QgsQuickFeatureModel::featureLayerPair() const
{
  return mFeatureLayerPair;
}

void QgsQuickFeatureModel::setFeatureLayerPair( const QgsQuickFeatureLayerPair &pair )
{
  beginResetModel();
  mFeatureLayerPair = pair;
  endResetModel();
  if ( mFeatureLayerPair.layer() )
  {
    mRememberedAttributes.resize( mFeatureLayerPair.layer()->fields().size() );
    mRememberedAttributes.fill( false );
  }
  emit featureLayerPairChanged();
  emit layerChanged();
}

void QgsQuickFeatureModel::setFeatureOnly( const QgsFeature &feature )
{
  if ( mFeatureLayerPair.feature() == feature )
    return;

  beginResetModel();
  // updates only feature TODO @vsklencar
  mFeatureLayerPair = QgsQuickFeatureLayerPair( feature, mFeatureLayerPair.layer() );
  endResetModel();
  emit featureLayerPairChanged();
}

void QgsQuickFeatureModel::setLayer( QgsVectorLayer *layer )
{
  if ( layer == mFeatureLayerPair.layer() )
    return;

  //mFeature.setLayer( layer );
  if ( mFeatureLayerPair.layer() )
  {
    //mFeature.setFeature( QgsFeature( mFeature.layer()->fields() ) );

    mRememberedAttributes.resize( layer->fields().size() );
    mRememberedAttributes.fill( false );
  }

  emit layerChanged();
}

QHash<int, QByteArray> QgsQuickFeatureModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AttributeName]  = QByteArrayLiteral( "AttributeName" );
  roles[AttributeValue] = QByteArrayLiteral( "AttributeValue" );
  roles[Field] = QByteArrayLiteral( "Field" );
  roles[RememberAttribute] = QByteArrayLiteral( "RememberAttribute" );

  return roles;
}


int QgsQuickFeatureModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return 0;
  else
    return mFeatureLayerPair.feature().attributes().count();
}

QVariant QgsQuickFeatureModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case AttributeName:
      return mFeatureLayerPair.layer()->attributeDisplayName( index.row() );
      break;

    case AttributeValue:
      return mFeatureLayerPair.feature().attribute( index.row() );
      break;

    case Field:
      return mFeatureLayerPair.layer()->fields().at( index.row() );
      break;

    case RememberAttribute:
      return mRememberedAttributes.at( index.row() );
      break;
  }

  return QVariant();
}

bool QgsQuickFeatureModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( data( index, role ) == value )
    return true;

  switch ( role )
  {
    case AttributeValue:
    {
      QVariant val( value );
      QgsField fld = mFeatureLayerPair.feature().fields().at( index.row() );

      if ( !fld.convertCompatible( val ) )
      {
        QgsMessageLog::logMessage( tr( "Value \"%1\" %4 could not be converted to a compatible value for field %2(%3)." ).arg( value.toString(), fld.name(), fld.typeName(), value.isNull() ? "NULL" : "NOT NULL" ) );
        return false;
      }
      bool success = mFeatureLayerPair.featureRef().setAttribute( index.row(), val );
      if ( success )
        emit dataChanged( index, index, QVector<int>() << role );
      return success;
      break;
    }

    case RememberAttribute:
    {
      mRememberedAttributes[ index.row() ] = value.toBool();
      emit dataChanged( index, index, QVector<int>() << role );
      break;
    }
  }

  return false;
}

bool QgsQuickFeatureModel::save()
{
  if ( !mFeatureLayerPair.layer() )
    return false;

  bool rv = true;

  if ( !startEditing() )
  {
    rv = false;
  }

  QgsFeature feat = mFeatureLayerPair.feature();
  if ( !mFeatureLayerPair.layer()->updateFeature( feat ) )
    QgsMessageLog::logMessage( tr( "Cannot update feature" ), QStringLiteral( "QgsQuick" ), Qgis::Warning );
  rv = commit();

  if ( rv )
  {
    QgsFeature feat;
    if ( mFeatureLayerPair.layer()->getFeatures( QgsFeatureRequest().setFilterFid( mFeatureLayerPair.feature().id() ) ).nextFeature( feat ) )
      setFeatureOnly( feat );
    else
      QgsMessageLog::logMessage( tr( "Feature %1 could not be fetched after commit" ).arg( mFeatureLayerPair.feature().id() ),
                                 QStringLiteral( "QgsQuick" ),
                                 Qgis::Warning );
  }
  return rv;
}

bool QgsQuickFeatureModel::deleteFeature()
{
  if ( !mFeatureLayerPair.layer() )
    return false;

  bool rv = true;

  if ( !startEditing() )
  {
    rv = false;
  }

  if ( !mFeatureLayerPair.layer()->deleteFeature( mFeatureLayerPair.feature().id() ) )
    QgsMessageLog::logMessage( tr( "Cannot delete feature" ), QStringLiteral( "QgsQuick" ), Qgis::Warning );
  rv = commit();

  return rv;
}

void QgsQuickFeatureModel::reset()
{
  if ( !mFeatureLayerPair.layer() )
    return;

  mFeatureLayerPair.layer()->rollBack();
}

bool QgsQuickFeatureModel::suppressFeatureForm() const
{
  if ( !mFeatureLayerPair.layer() )
    return false;

  return mFeatureLayerPair.layer()->editFormConfig().suppress();
}

void QgsQuickFeatureModel::resetAttributes()
{
  if ( !mFeatureLayerPair.layer() )
    return;

  QgsExpressionContext expressionContext = mFeatureLayerPair.layer()->createExpressionContext();
  expressionContext.setFeature( mFeatureLayerPair.feature() );

  QgsFields fields = mFeatureLayerPair.layer()->fields();

  beginResetModel();
  for ( int i = 0; i < fields.count(); ++i )
  {
    if ( !mRememberedAttributes.at( i ) )
    {
      if ( !fields.at( i ).defaultValueDefinition().expression().isEmpty() )
      {
        QgsExpression exp( fields.at( i ).defaultValueDefinition().expression() );
        exp.prepare( &expressionContext );
        if ( exp.hasParserError() )
          QgsMessageLog::logMessage( tr( "Default value expression for %1:%2 has parser error: %3" ).arg( mFeatureLayerPair.layer()->name(),
                                     fields.at( i ).name(), exp.parserErrorString() ), QStringLiteral( "QgsQuick" ),
                                     Qgis::Warning );

        QVariant value = exp.evaluate( &expressionContext );

        if ( exp.hasEvalError() )
          QgsMessageLog::logMessage( tr( "Default value expression for %1:%2 has evaluation error: %3" ).arg( mFeatureLayerPair.layer()->name(),
                                     fields.at( i ).name(), exp.evalErrorString() ),
                                     QStringLiteral( "QgsQuick" ),
                                     Qgis::Warning );

        mFeatureLayerPair.feature().setAttribute( i, value );
      }
      else
      {
        mFeatureLayerPair.feature().setAttribute( i, QVariant() );
      }
    }
  }
  endResetModel();
}

void QgsQuickFeatureModel::create()
{
  if ( !mFeatureLayerPair.layer() )
    return;

  startEditing();
  QgsFeature feat = mFeatureLayerPair.feature();
  if ( !mFeatureLayerPair.layer()->addFeature( feat ) )
  {
    QgsMessageLog::logMessage( tr( "Feature could not be added" ), QStringLiteral( "QgsQuick" ), Qgis::Critical );
  }
  commit();
}

bool QgsQuickFeatureModel::commit()
{
  if ( !mFeatureLayerPair.layer()->commitChanges() )
  {
    QgsMessageLog::logMessage( tr( "Could not save changes. Rolling back." ), QStringLiteral( "QgsQuick" ), Qgis::Critical );
    mFeatureLayerPair.layer()->rollBack();
    return false;
  }
  else
  {
    return true;
  }
}

bool QgsQuickFeatureModel::startEditing()
{
  // Already an edit session active
  if ( mFeatureLayerPair.layer()->editBuffer() )
    return true;

  if ( !mFeatureLayerPair.layer()->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), QStringLiteral( "QgsQuick" ), Qgis::Warning );
    return false;
  }
  else
  {
    return true;
  }
}

QVector<bool> QgsQuickFeatureModel::rememberedAttributes() const
{
  return mRememberedAttributes;
}
