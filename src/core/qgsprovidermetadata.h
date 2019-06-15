/***************************************************************************
                    qgsprovidermetadata.h  -  Metadata class for
                    describing a data provider.
                             -------------------
    begin                : Sat Jan 10 2004
    copyright            : (C) 2004 by Gary E.Sherman
    email                : sherman at mrcc.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPROVIDERMETADATA_H
#define QGSPROVIDERMETADATA_H


#include <QString>
#include <QVariantMap>
#include <QMap>
#include <QList>
#include <memory>
#include <QPair>

#include "qgis_sip.h"
#include "qgsdataprovider.h"
#include "qgis_core.h"
#include <functional>
#include "qgsvectorlayerexporter.h"
#include "qgsfields.h"

class QgsDataItem;
class QgsDataItemProvider;
class QgsTransaction;

class QgsRasterDataProvider;


/**
 * \ingroup core
 * Holds data provider key, description, and associated shared library file or function pointer information.
 *
 * Provider metadata refers either to providers which are loaded via libraries or
 * which are native providers that are included in the core QGIS installation
 * and accessed through function pointers.
 *
 * For library based providers, the metadata class is used in a lazy load
 * implementation in QgsProviderRegistry.  To save memory, data providers
 * are only actually loaded via QLibrary calls if they're to be used.  (Though they're all
 * iteratively loaded once to get their metadata information, and then
 * unloaded when the QgsProviderRegistry is created.)  QgsProviderMetadata
 * supplies enough information to be able to later load the associated shared
 * library object.
 *
 */
class CORE_EXPORT QgsProviderMetadata
{
  public:

    /**
     * Typedef for data provider creation function.
     * \since QGIS 3.0
     * \deprecated QGIS 3.10
     */
    SIP_SKIP typedef std::function < QgsDataProvider*( const QString &, const QgsDataProvider::ProviderOptions & ) > CreateDataProviderFunction;

    /**
     * Constructor for provider metadata
     * \param provider key
     * \param provider description
     * \param provider plugin library name
     *
     * \deprecated QGIS 3.10
     */
    QgsProviderMetadata( const QString &_key, const QString &_description, const QString &_library );

    /**
     * Constructor for provider metadata
     * \param _key provider key
     * \param _description provider description
     */
    QgsProviderMetadata( const QString &_key, const QString &_description );

    /**
     * Metadata for provider with direct provider creation function pointer, where
     * no library is involved.
     * \since QGIS 3.0
     * \deprecated QGIS 3.10
     */
#ifndef SIP_RUN
    QgsProviderMetadata( const QString &key, const QString &description, const QgsProviderMetadata::CreateDataProviderFunction &createFunc );
#else
    QgsProviderMetadata( const QString &key, const QString &description, SIP_PYCALLABLE / AllowNone / );
    % MethodCode

    // Make sure the callable doesn't get garbage collected, this is needed because refcount for a2 is 0
    // and the creation function pointer is passed to the metadata and it needs to be kept in memory.
    Py_INCREF( a2 );

    Py_BEGIN_ALLOW_THREADS

    sipCpp = new QgsProviderMetadata( *a0, *a1, [a2]( const QString &dataSource, const QgsDataProvider::ProviderOptions &providerOptions ) -> QgsDataProvider*
    {
      QgsDataProvider *provider;
      provider = nullptr;
      PyObject *sipResObj;
      SIP_BLOCK_THREADS

      sipResObj = sipCallMethod( nullptr, a2, "DD", new QString( dataSource ), sipType_QString, nullptr, new QgsDataProvider::ProviderOptions( providerOptions ), sipType_QgsDataProvider_ProviderOptions, NULL );

      if ( sipResObj )
      {
        if ( sipCanConvertToType( sipResObj, sipType_QgsDataProvider, SIP_NOT_NONE ) )
        {
          int state0;
          int sipIsErr = 0;
          provider = reinterpret_cast<QgsDataProvider *>( sipConvertToType( sipResObj, sipType_QgsDataProvider, nullptr, SIP_NOT_NONE, &state0, &sipIsErr ) );
          if ( sipIsErr != 0 )
          {
            sipReleaseType( provider, sipType_QgsDataProvider, state0 );
            provider = nullptr;
          }
        }
      }
      SIP_UNBLOCK_THREADS
      return provider;
    } );

    Py_END_ALLOW_THREADS

    % End
#endif

    //! dtor
    virtual ~QgsProviderMetadata();

    /**
     * This returns the unique key associated with the provider
     *
     * This key string is used for the associative container in QgsProviderRegistry
     */
    QString key() const;

    /**
     * This returns descriptive text for the provider
     *
     * This is used to provide a descriptive list of available data providers.
     */
    QString description() const;

    /**
     * This returns the library file name
     *
     * This is used to QLibrary calls to load the data provider (only for dynamically loaded libraries)
     *
     * \deprecated QGIS 3.10
     */
    QString library() const;

    /**
     * Returns a pointer to the direct provider creation function, if supported
     * by the provider.
     * \note not available in Python bindings
     * \since QGIS 3.0
     * \deprecated QGIS 3.10
     */
    SIP_SKIP CreateDataProviderFunction createFunction() const;

    /**
      * Initialize the provider
      * \since QGIS 3.10
      */
    virtual void initProvider();

    /**
     * Cleanup the provider
     * \since QGIS 3.10
     */
    virtual void cleanupProvider();

    /**
     * Builds the list of vector file filter strings
     *
     * Queries for a list of supported vector formats
     * Suitable for use in a QFileDialog::getOpenFileNames() call.
     *
     * \since QGIS 3.10
     */
    virtual QString fileVectorFilters( );

    /**
     * Builds the list of raster file filter strings
     *
     * Queries for a list of supported raster formats
     * Suitable for use in a QFileDialog::getOpenFileNames() call.
     *
     * \since QGIS 3.10
     */
    virtual QString fileRasterFilters( );

    /**
     * Builds the list of mesh file filter strings
     *
     * Queries for a list of supported mesh formats; one list to be used for meshes
     * and also one for datasets. Suitable for use in a
     * QFileDialog::getOpenFileNames() call.
     *
     * \since QGIS 3.10
     */
    virtual void fileMeshFilters( QString &mesh, QString &datasets );

    /**
     * Class factory to return a pointer to a newly created QgsDataProvider object
     * \since QGIS 3.0
     */
    virtual QgsDataProvider *createProvider( const QString *uri, const QgsDataProvider::ProviderOptions &options );

    /**
     * Interface for data provider function related to vector layers
     * \since QGIS 3.10
     */
    virtual QgsVectorLayerExporter::ExportError createEmptyLayer(
      const QString &uri,
      const QgsFields &fields,
      QgsWkbTypes::Type wkbType,
      const QgsCoordinateReferenceSystem &srs,
      bool overwrite,
      QMap<int, int> &oldToNewAttrIdxMap,
      QString &errorMessage,
      const QMap<QString, QVariant> *options );

    /**
     * Interface for data provider function related to raster layers
     * \since QGIS 3.10
     */
    virtual QgsRasterDataProvider *createRasterDataProvider(
      const QString &uri,
      const QString &format,
      int nBands,
      Qgis::DataType type,
      int width,
      int height,
      double *geoTransform,
      const QgsCoordinateReferenceSystem &crs,
      const QStringList &createOptions = QStringList() );

    /**
     * Interface for data provider function related to raster layers
     * \since QGIS 3.10
     */
    virtual QList<QPair<QString, QString> > *pyramidResamplingMethods();

    /**
     * Decode URI
     * \since QGIS 3.10
     */
    virtual QVariantMap decodeUri( const QString &uri );

    /**
     * Data item providers
     * \see QgsProviderGuiMetadata::dataItemGuiProviders()
     * \since QGIS 3.10
     */
    virtual QList< QgsDataItemProvider * > *dataItemProviders() const;

    /**
     * Interface for data provider functions related to CRUD for layer styles
     * \since QGIS 3.10
     */
    virtual int listStyles( const QString &uri, QStringList &ids, QStringList &names,
                            QStringList &descriptions, QString &errCause );

    /**
     * Interface for data provider functions related to CRUD for layer styles
     * \since QGIS 3.10
     */
    virtual QString getStyleById( const QString &uri, QString styleId, QString &errCause );

    /**
     * Interface for data provider functions related to CRUD for layer styles
     * \since QGIS 3.10
     */
    virtual bool deleteStyleById( const QString &uri, QString styleId, QString &errCause );

    /**
     * Interface for data provider functions related to CRUD for layer styles
     * \since QGIS 3.10
     */
    virtual bool saveStyle( const QString &uri, const QString &qmlStyle, const QString &sldStyle,
                            const QString &styleName, const QString &styleDescription,
                            const QString &uiFileContent, bool useAsDefault, QString &errCause );

    /**
     * Interface for data provider functions related to CRUD for layer styles
     * \since QGIS 3.10
     */
    virtual QString loadStyle( const QString &uri, QString &errCause );

    /**
     * Interface for data provider functions related to data management (databases)
     * \since QGIS 3.10
     */
    virtual bool createDb( const QString &dbPath, QString &errCause );

    /**
     * Interface for data provider functions related to data management (databases)
     * \since QGIS 3.10
     */
    virtual QgsTransaction *createTransaction( const QString &connString );


  private:

    /// unique key for data provider
    QString mKey;

    /// associated terse description
    QString mDescription;

    /// file path
    /// deprecated QGIS 3.10
    QString mLibrary;

    CreateDataProviderFunction mCreateFunction = nullptr;
};

#endif //QGSPROVIDERMETADATA_H
