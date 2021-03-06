/*****************************************************************************/
/**
 *  @file   ClusterMapping.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ClusterMapping.cpp 354 2013-07-05 09:35:41Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ClusterMapping.h"
#include <cstring>
#include <kvs/AnyValueArray>
#include <kvs/ValueArray>
#include <kvs/Value>


namespace kvsoceanvis
{

namespace pcs
{

ClusterMapping::ClusterMapping()
{
}

ClusterMapping::ClusterMapping( const kvs::ObjectBase* object )
{
    this->exec( object );
}

ClusterMapping::SuperClass* ClusterMapping::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return( NULL );
    }

    const kvs::TableObject* table = kvs::TableObject::DownCast( object );
    const size_t nrows = table->numberOfRows();
    const size_t ncolumns = table->numberOfColumns();

    // Number of clusters.
    /* NOTE: The last column of the table repesents as the cluster IDs and the IDs are expressed as
     * sequential integer numbers from zero. Therefore, it can be assumed that the maximum number + 1
     * of the last column of the table is equal to the number of clusters.
     */
    const size_t index = ncolumns - 1;
    const size_t nclusters = static_cast<size_t>( table->maxValue( index ) + 1 );
    const size_t naxes = index;

    // Cluster parameters.
    size_t* counter = new size_t [nclusters];
    kvs::ValueArray<kvs::Real64>* min_values = new kvs::ValueArray<kvs::Real64> [nclusters];
    kvs::ValueArray<kvs::Real64>* max_values = new kvs::ValueArray<kvs::Real64> [nclusters];

    // Initialize the parameters.
    std::memset( counter, 0, sizeof(size_t) * nclusters );
    for ( size_t i = 0; i < nclusters; i++ )
    {
        min_values[i].allocate( naxes );
        max_values[i].allocate( naxes );
        for ( size_t j = 0; j < naxes; j++ )
        {
            min_values[i][j] = kvs::Value<kvs::Real64>::Max();
            max_values[i][j] = kvs::Value<kvs::Real64>::Min();
        }
    }

    // Cluster mapping.
    const kvs::AnyValueArray& IDs = table->column( index );
    for ( size_t i = 0; i < nrows; i++ )
    {
        const size_t id = IDs.at<size_t>(i);
        for ( size_t j = 0; j < naxes; j++ )
        {
            const kvs::AnyValueArray& column = table->column(j);
            const kvs::Real64 value = column.at<kvs::Real64>( i );
            min_values[id][j] = kvs::Math::Min( min_values[id][j], value );
            max_values[id][j] = kvs::Math::Max( max_values[id][j], value );
        }
        counter[id]++;
    }

    // Set the clusters.
    for ( size_t i = 0; i < nclusters; i++ )
    {
        Cluster cluster;
        cluster.setID( i );
        cluster.setCounter( counter[i] );
        cluster.setMinValues( min_values[i] );
        cluster.setMaxValues( max_values[i] );
        SuperClass::addCluster( cluster );
    }

    delete [] counter;
    delete [] min_values;
    delete [] max_values;

    // Sorting.
    SuperClass::sortClusters();

    // Number of axes.
    SuperClass::m_naxes = naxes;
    SuperClass::setNumberOfColumns( naxes );

    // Number of points.
    SuperClass::m_npoints = nrows;
    SuperClass::setNumberOfRows( nrows );

    // Min/Max value and label.
/*
    for ( size_t i = 0; i < naxes; i++ )
    {
        SuperClass::m_min_values.push_back( table->minValue(i) );
        SuperClass::m_max_values.push_back( table->maxValue(i) );
        SuperClass::m_labels.push_back( table->label(i) );
    }
*/
    SuperClass::setMinValues( table->minValues() );
    SuperClass::setMaxValues( table->maxValues() );
    SuperClass::setLabels( table->labels() );

    // Min/Max range.
//    SuperClass::m_min_ranges = m_min_values;
//    SuperClass::m_max_ranges = m_max_values;
    SuperClass::setMinRanges( table->minValues() );
    SuperClass::setMaxRanges( table->maxValues() );

    return this;
}

} // end of namespace pcs

} // end of namespace kvsoceanvis
