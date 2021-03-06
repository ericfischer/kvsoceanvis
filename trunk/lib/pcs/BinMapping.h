/*****************************************************************************/
/**
 *  @file   BinMapping.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BinMapping.h 326 2012-12-22 06:19:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSOCEANVIS__PCS__BIN_MAPPING_H_INCLUDE
#define KVSOCEANVIS__PCS__BIN_MAPPING_H_INCLUDE

#include <kvs/Module>
#include <kvs/FilterBase>
#include <kvs/TableObject>
#include "BinMapObject.h"


namespace kvsoceanvis
{

namespace pcs
{

/*===========================================================================*/
/**
 *  @brief  Binning class.
 */
/*===========================================================================*/
class BinMapping : public kvs::FilterBase, public pcs::BinMapObject
{
    kvsModuleName( kvsoceanvis::pcs::BinMapping );
    kvsModuleCategory( Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( pcs::BinMapObject );

public:

    BinMapping();
    BinMapping( const kvs::ObjectBase* object );

public:

    SuperClass* exec( const kvs::ObjectBase* object );

protected:

    size_t get_nbins_by_sturges_formula( const kvs::TableObject* table, const size_t index );
    size_t get_nbins_by_scott_choice( const kvs::TableObject* table, const size_t index );
    kvs::Real64 get_bin_width_by_scott_choice( const kvs::AnyValueArray& array );
    kvs::Real64 get_standard_deviation( const kvs::AnyValueArray& array );
    kvs::Real64 get_mean( const kvs::AnyValueArray& array );
    kvs::Real64 get_value( const kvs::AnyValueArray& array, const size_t index );
};

} // end of namespace pcs

} // end of namespace kvsoceanvis

#endif // KVSOCEANVIS__PCS__BIN_MAPPING_H_INCLUDE
