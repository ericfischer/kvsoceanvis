/*****************************************************************************/
/**
 *  @file   DegreeToDistance.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DegreeToDistance.cpp 326 2012-12-22 06:19:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "DegreeToDistance.h"


namespace kvsoceanvis
{

namespace util
{

DegreeToDistance::DegreeToDistance(
    const kvs::StructuredVolumeObject* volume,
    const float longitude_scale,
    const float latitude_scale,
    const float depth_scale ):
    m_longitude_scale( longitude_scale ),
    m_latitude_scale( latitude_scale ),
    m_depth_scale( depth_scale )
{
    this->exec( volume );
}

DegreeToDistance::SuperClass* DegreeToDistance::exec( const kvs::ObjectBase* object )
{
    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( object );

    if ( volume->gridType() != kvs::StructuredVolumeObject::Rectilinear )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not rectilinear grid.");
        return( NULL );
    }

//    kvs::ValueArray<kvs::Real32> coords;
//    coords.deepCopy( volume->coords() );
    kvs::ValueArray<kvs::Real32> coords = volume->coords().clone();

    kvs::Real32* xcoords = coords.pointer();
    kvs::Real32* ycoords = xcoords + volume->resolution().x();
    kvs::Real32* zcoords = ycoords + volume->resolution().y();
    for ( size_t i = 0; i < volume->resolution().x(); i++ ) xcoords[i] *= m_longitude_scale;
    for ( size_t i = 0; i < volume->resolution().y(); i++ ) ycoords[i] *= m_latitude_scale;
    for ( size_t i = 0; i < volume->resolution().z(); i++ ) zcoords[i] *= m_depth_scale;

    kvs::Vector3f min_coord = volume->minObjectCoord();
    kvs::Vector3f max_coord = volume->maxObjectCoord();
    min_coord.x() *= m_longitude_scale;
    max_coord.x() *= m_longitude_scale;
    min_coord.y() *= m_latitude_scale;
    max_coord.y() *= m_latitude_scale;
    min_coord.z() *= m_depth_scale;
    max_coord.z() *= m_depth_scale;

    SuperClass::setCoords( coords );
    SuperClass::setMinMaxObjectCoords( min_coord, max_coord );
    SuperClass::setMinMaxExternalCoords( min_coord, max_coord );
    SuperClass::setGridType( volume->gridType() );
    SuperClass::setVeclen( volume->veclen() );
    SuperClass::setResolution( volume->resolution() );
    SuperClass::setValues( volume->values() );
    SuperClass::setMinMaxValues( volume->minValue(), volume->maxValue() );

    return this;
}

} // end of namespace util

} // end of namespace kvsoceanvis
