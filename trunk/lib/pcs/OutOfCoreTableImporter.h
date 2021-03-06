/*****************************************************************************/
/**
 *  @file   OutOfCoreTableImporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OutOfCoreTableImporter.h 326 2012-12-22 06:19:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSOCEANVIS__PCS__OUT_OF_CORE_TABLE_IMPORTER_H_INCLUDE
#define KVSOCEANVIS__PCS__OUT_OF_CORE_TABLE_IMPORTER_H_INCLUDE

#include <kvs/ImporterBase>
#include <kvs/Module>
#include <kvs/KVSMLObjectTable>
#include "OutOfCoreTableObject.h"


namespace kvsoceanvis
{

namespace pcs
{

/*===========================================================================*/
/**
 *  @brief  Out-of-core table importer class.
 */
/*===========================================================================*/
class OutOfCoreTableImporter : public kvs::ImporterBase, public pcs::OutOfCoreTableObject
{
    kvsModuleName( kvsoceanvis::pcs::OutOfCoreTableImporter );
    kvsModuleCategory( Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( pcs::OutOfCoreTableObject );

public:

    OutOfCoreTableImporter();
    OutOfCoreTableImporter( const std::string& filename, const kvs::UInt64 cache_size = 0 );

public:

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:

    void import( const kvs::KVSMLObjectTable* kvsml, const std::string& filename );
};

} // end of namespace pcs

} // end of namespace kvsoceanvis

#endif // KVSOCEANVIS__PCS__OUT_OF_CORE_TABLE_IMPORTER_H_INCLUDE
