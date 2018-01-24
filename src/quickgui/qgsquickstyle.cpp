/***************************************************************************
  qgsquickstyle.cpp
  --------------------------------------
  Date                 : Nov 2017
  Copyright            : (C) 2017 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickstyle.h"

QgsQuickStyle::QgsQuickStyle( QObject *parent ):
  QObject( parent )
  , mDevicePixels( 1.0f )
  , mUseVectorIcons( false )
{
}

qreal QgsQuickStyle::devicePixels() const
{
  return mDevicePixels;
}

QString QgsQuickStyle::themeDir() const
{
  return mThemeDir;
}

void QgsQuickStyle::setDevicePixels( qreal dp )
{
  if ( dp != mDevicePixels )
  {
    mDevicePixels = dp;
    emit devicePixelsChanged();
  }
}

void QgsQuickStyle::setThemeDir( QString dir )
{
  if ( dir != mThemeDir )
  {
    mThemeDir = dir;
    emit themeDirChanged();
  }
}

bool QgsQuickStyle::useVectorIcons() const
{
  return mUseVectorIcons;
}

void QgsQuickStyle::setUseVectorIcons( bool use )
{
  if ( use != mUseVectorIcons )
  {
    mUseVectorIcons = use;
    emit useVectorIconsChanged();
  }
}
