/*****************************************************************************/
/**
 *  @file   MultiBinMapObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MultiBinMapObject.cpp 395 2014-10-24 09:00:30Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "MultiBinMapObject.h"
#include <kvs/Vector3>
#include <kvs/Math>
#include <iostream>
#include <fstream>


namespace kvsoceanvis
{

namespace pcs
{

MultiBinMapObject::MultiBinMapObject()
{
    m_npoints = 0;
}

size_t MultiBinMapObject::npoints() const
{
    return m_npoints;
}

const MultiBinMapObject::BinList& MultiBinMapObject::binList() const
{
    return m_bin_list;
}

const kvs::ValueArray<kvs::UInt32>& MultiBinMapObject::nbins() const
{
    return m_nbins;
}

size_t MultiBinMapObject::naxes() const
{
    return m_nbins.size();
}

void MultiBinMapObject::setMinRange( const size_t column_index, const kvs::Real64 range )
{
    const kvs::Real64 min_value = this->minValue( column_index );
    const kvs::Real64 max_range = BaseClass::maxRange( column_index );

    const kvs::Real64 min_range_old = BaseClass::minRange( column_index );
    const kvs::Real64 min_range_new = kvs::Math::Clamp( range, min_value, max_range );

    if ( kvs::Math::Equal( min_range_old, min_range_new ) ) return;
    BaseClass::setMinRange( column_index, min_range_new );
}

void MultiBinMapObject::setMaxRange( const size_t column_index, const kvs::Real64 range )
{
    const kvs::Real64 min_range = BaseClass::minRange( column_index );
    const kvs::Real64 max_value = this->maxValue( column_index );

    const kvs::Real64 max_range_old = BaseClass::maxRange( column_index );
    const kvs::Real64 max_range_new = kvs::Math::Clamp( range, min_range, max_value );

    if ( kvs::Math::Equal( max_range_old, max_range_new ) ) return;
    BaseClass::setMaxRange( column_index, max_range_new );
}

void MultiBinMapObject::setRange( const size_t column_index, const kvs::Real64 min_range, const kvs::Real64 max_range )
{
    this->setMinRange( column_index, min_range );
    this->setMaxRange( column_index, max_range );
}

void MultiBinMapObject::moveMinRange( const size_t column_index, const kvs::Real64 drange )
{
    const kvs::Real64 min_range = this->minRange( column_index );
    this->setMinRange( column_index, min_range + drange );
}

void MultiBinMapObject::moveMaxRange( const size_t column_index, const kvs::Real64 drange )
{
    const kvs::Real64 max_range = this->maxRange( column_index );
    this->setMaxRange( column_index, max_range + drange );
}

void MultiBinMapObject::moveRange( const size_t column_index, const kvs::Real64 drange )
{
    const kvs::Real64 min_range = this->minRange( column_index );
    const kvs::Real64 max_range = this->maxRange( column_index );
    const kvs::Real64 min_value = this->minValue( column_index );
    const kvs::Real64 max_value = this->maxValue( column_index );

    const kvs::Real64 range_width = max_range - min_range;
    if ( max_range + drange > max_value )
    {
        this->setMinRange( column_index, max_value - range_width );
        this->setMaxRange( column_index, max_value );
    }
    else if ( min_range + drange < min_value )
    {
        this->setMinRange( column_index, min_value );
        this->setMaxRange( column_index, min_value + range_width );
    }
    else
    {
        this->setMinRange( column_index, min_range + drange );
        this->setMaxRange( column_index, max_range + drange );
    }
}

void MultiBinMapObject::resetRange( const size_t column_index )
{
    this->setMinRange( column_index, this->minValue(column_index) );
    this->setMaxRange( column_index, this->maxValue(column_index) );
}

void MultiBinMapObject::resetRange()
{
    const size_t naxes = this->naxes();
    for ( size_t i = 0; i < naxes; i++ )
    {
//        m_max_ranges[i] = this->maxValue(i);
//        m_min_ranges[i] = this->minValue(i);
        BaseClass::setMaxRange( i, this->maxValue(i) );
        BaseClass::setMinRange( i, this->minValue(i) );
//        this->setMaxRange( i, this->maxValue(i) );
//        this->setMinRange( i, this->minValue(i) );
    }
}

kvs::ObjectBase::ObjectType MultiBinMapObject::objectType() const
{
    return kvs::ObjectBase::Table;
}

MultiBinMapObject::Bin::Bin( const kvs::ValueArray<kvs::UInt16>& indices )
{
    m_counter = 1;
    m_indices = indices;
}

void MultiBinMapObject::Bin::count()
{
    m_counter++;
}

size_t MultiBinMapObject::Bin::counter() const
{
    return m_counter;
}

const kvs::ValueArray<kvs::UInt16>& MultiBinMapObject::Bin::indices() const
{
    return m_indices;
}

bool operator < ( const MultiBinMapObject::Bin& b0, const MultiBinMapObject::Bin& b1 )
{
    return b0.counter() < b1.counter();
}

bool operator == ( const MultiBinMapObject::Bin& b0, const MultiBinMapObject::Bin& b1 )
{
    return b0.indices() == b1.indices();
}

} // end of namespace pcs

} // end of namespace kvsoceanvis
