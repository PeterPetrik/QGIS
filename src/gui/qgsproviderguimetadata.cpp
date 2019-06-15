/***************************************************************************
   qgsproviderguimetadata.cpp
   --------------------------
    begin                : June 4th 2019
    copyright            : (C) 2019 by Peter Petrik
    email                : zilolv at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsproviderguimetadata.h"
#include "qgsdataitemguiprovider.h"
#include "qgsprojectstorageguiprovider.h"
#include "qgssourceselectprovider.h"

QgsProviderGuiMetadata::QgsProviderGuiMetadata( const QString &key, const QString &description )
  : mKey( key ), mDescription( description )
{
}

QgsProviderGuiMetadata::~QgsProviderGuiMetadata() = default;

QList<QgsDataItemGuiProvider *> *QgsProviderGuiMetadata::dataItemGuiProviders()
{
  return nullptr;
}

QList<QgsProjectStorageGuiProvider *> *QgsProviderGuiMetadata::projectStorageGuiProviders()
{
  return nullptr;
}

QList<QgsSourceSelectProvider *> *QgsProviderGuiMetadata::sourceSelectProviders()
{
  return nullptr;
}

QString QgsProviderGuiMetadata::key() const
{
  return mKey;
}

QString QgsProviderGuiMetadata::description() const
{
  return mDescription;
}

void QgsProviderGuiMetadata::registerGui( QMainWindow * )
{
}

