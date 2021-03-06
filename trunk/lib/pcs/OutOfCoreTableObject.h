/*****************************************************************************/
/**
 *  @file   OutOfCoreTableObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OutOfCoreTableObject.h 326 2012-12-22 06:19:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSOCEANVIS__PCS__OUT_OF_CORE_TABLE_OBJECT_H_INCLUDE
#define KVSOCEANVIS__PCS__OUT_OF_CORE_TABLE_OBJECT_H_INCLUDE

#include <vector>
#include <string>
#include <cstdio>
#include <kvs/Module>
#include <kvs/TableObject>


namespace kvsoceanvis
{

namespace pcs
{

/*===========================================================================*/
/**
 *  @brief  Out-of-core table class.
 */
/*===========================================================================*/
class OutOfCoreTableObject : public kvs::TableObject
{
    kvsModuleName( kvsoceanvis::pcs::OutOfCoreTableObject );
    kvsModuleCategory( Object );
    kvsModuleBaseClass( kvs::TableObject );

protected:

    std::vector<std::string> m_column_types; ///< column types
    std::vector<std::string> m_column_formats; ///< column formats
    std::vector<std::string> m_column_files; ///< column files
    mutable std::vector<FILE*> m_column_file_pointers; // column file pointers
    bool m_cache_enabled; ///< enable chache machanism
    kvs::UInt64 m_cache_size; ///< cache size [byte]
    mutable size_t m_cache_index; ///< start index of cached data
    mutable size_t m_cache_nrows; ///< number of cached rows
    mutable Columns m_cache_columns; ///< cached columns

public:

    OutOfCoreTableObject();
    virtual ~OutOfCoreTableObject();

public:

    void enableCache( const kvs::UInt64 cache_size );
    void disableCache();
    void clearCache();
    void fetch() const;
    void openColumnFiles() const;
    void closeColumnFiles() const;

    const std::string& columnType( const size_t index ) const;
    const std::string& columnFormat( const size_t index ) const;
    const std::string& columnFile( const size_t index ) const;
    kvs::AnyValueArray column( const size_t index ) const;
    ObjectType objectType() const;

    kvs::Real64 readValue( const size_t row_index, const size_t column_index ) const;
};

} // end of namespace pcs

} // end of namespace kvsoceanvis

#endif // KVSOCEANVIS__PCS__OUT_OF_CORE_TABLE_OBJECT_H_INCLUDE
