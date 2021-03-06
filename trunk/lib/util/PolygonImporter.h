/*****************************************************************************/
/**
 *  @file   PolygonImporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonImporter.h 287 2012-06-15 14:20:06Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSOCEANVIS__UTIL__POLYGON_IMPORTER_H_INCLUDE
#define KVSOCEANVIS__UTIL__POLYGON_IMPORTER_H_INCLUDE

#include <kvs/ImporterBase>
#include <kvs/PolygonObject>
#include <kvs/Gis>


namespace kvsoceanvis
{

namespace util
{

class PolygonImporter : public kvs::ImporterBase, public kvs::PolygonObject
{
public:

    PolygonImporter( const std::string& filename );
    PolygonImporter( const kvs::FileFormatBase* file_format );

public:

    kvs::PolygonObject* exec( const kvs::FileFormatBase* file_format );

private:

    void import( const kvs::Gis* gis );
};

} // end of namespace util

} // end of namespace kvsoceanvis

#endif // KVSOCEANVIS__UTIL__POLYGON_IMPORTER_H_INCLUDE
