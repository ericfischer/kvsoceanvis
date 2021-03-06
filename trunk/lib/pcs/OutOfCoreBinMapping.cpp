/*****************************************************************************/
/**
 *  @file   OutOfCoreBinMapping.cpp
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OutOfCoreBinMapping.cpp 326 2012-12-22 06:19:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "OutOfCoreBinMapping.h"
#include <kvs/AnyValueArray>


namespace kvsoceanvis
{

namespace pcs
{

OutOfCoreBinMapping::OutOfCoreBinMapping()
{
}

OutOfCoreBinMapping::OutOfCoreBinMapping( const kvs::ObjectBase* object )
{
    this->exec( object );
}

OutOfCoreBinMapping::SuperClass* OutOfCoreBinMapping::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const pcs::OutOfCoreTableObject* table = reinterpret_cast<const pcs::OutOfCoreTableObject*>( object );

    // Calculate number of binns for each axis.
    const size_t ncolumns = table->numberOfColumns();
    kvs::ValueArray<kvs::UInt32> nbins( ncolumns );
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        const size_t n = this->get_nbins_by_sturges_formula( table, i );
//        const size_t n = this->get_nbins_by_scott_choice( table, i );
        nbins[i] = n;
    }

    // Bin mapping (2D binning).
    for ( size_t i = 0; i < ncolumns - 1; i++ )
    {
        const kvs::AnyValueArray& x = table->column(i);
        const kvs::AnyValueArray& y = table->column(i+1);

        const kvs::Real64 xmin = table->minRange(i);
        const kvs::Real64 xmax = table->maxRange(i);
        const kvs::Real64 ymin = table->minRange(i+1);
        const kvs::Real64 ymax = table->maxRange(i+1);

        // Count.
        const kvs::UInt32 xsize = nbins[i];
        const kvs::UInt32 ysize = nbins[i+1];
        kvs::ValueArray<kvs::UInt32> bin_map( xsize * ysize );
        bin_map.fill( 0x00 );

        const size_t nvalues = x.size();
        for ( size_t j = 0; j < nvalues; j++ )
        {
            const size_t xindex = size_t( ( xsize - 1 ) * ( this->get_value(x,j) - xmin ) / ( xmax - xmin ) );
            const size_t yindex = size_t( ( ysize - 1 ) * ( this->get_value(y,j) - ymin ) / ( ymax - ymin ) );

            const size_t index = xindex + yindex * xsize;
            bin_map[index]++;
        }

        SuperClass::addBinMap( bin_map );
    }

    return this;
}

size_t OutOfCoreBinMapping::get_nbins_by_scott_choice( const pcs::OutOfCoreTableObject* table, const size_t index )
{
    const kvs::AnyValueArray& array = table->column(index);
    const kvs::Real64 h = get_bin_width_by_scott_choice(array);
    const kvs::Real64 min = table->minValue(index);
    const kvs::Real64 max = table->maxValue(index);
    return size_t( std::ceil( ( max - min ) / h ) );
}

size_t OutOfCoreBinMapping::get_nbins_by_sturges_formula( const pcs::OutOfCoreTableObject* table, const size_t index )
{
    const kvs::AnyValueArray& array = table->column(index);
    const kvs::Real64 n = array.size();
//    return std::ceil( log2(n) + 1 );
    return size_t( std::ceil( std::log(n) / std::log(2.0) + 1 ) );
}

} // end of namespace pcs

} // end of namespace kvsoceanvis
