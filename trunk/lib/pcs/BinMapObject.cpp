/*****************************************************************************/
/**
 *  @file   BinMapObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BinMapObject.cpp 326 2012-12-22 06:19:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "BinMapObject.h"
#include <kvs/Vector3>


namespace kvsoceanvis
{

namespace pcs
{

BinMapObject::BinMapObject()
{
}

const BinMapObject::BinMap& BinMapObject::binMap( const size_t index ) const
{
    return m_bin_maps[index];
}

kvs::UInt32 BinMapObject::nbins( const size_t index ) const
{
    return m_nbins[index];
}

size_t BinMapObject::nmaps() const
{
    return m_bin_maps.size();
}

size_t BinMapObject::naxes() const
{
    return m_nbins.size();
}

kvs::ObjectBase::ObjectType BinMapObject::objectType() const
{
    return kvs::ObjectBase::UnknownObject;
}

void BinMapObject::addBinMap( const BinMap& bin_map )
{
    m_bin_maps.push_back( bin_map );
}

} // end of namespace pcs

} // end of namespace kvsoceanvis
