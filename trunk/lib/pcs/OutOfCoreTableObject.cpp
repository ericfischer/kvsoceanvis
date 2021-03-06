/*****************************************************************************/
/**
 *  @file   OutOfCoreTableObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OutOfCoreTableObject.cpp 355 2013-07-05 09:37:24Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "OutOfCoreTableObject.h"
#include <cstring>
#include <kvs/Vector3>
#include <kvs/AnyValueArray>
#include <Core/FileFormat/KVSML/DataArray.h>


namespace
{

template <typename T>
const kvs::Real64 ReadExternalData(
    const size_t index,
    const std::string& format,
    FILE* file_pointer )
{
    if ( format == "binary" )
    {
        T value = T(0);
        fseek( file_pointer, sizeof(T) * index, SEEK_SET );
        fread( &value, sizeof(T), 1, file_pointer );
        return kvs::Real64( value );
    }
    else if ( format == "ascii" )
    {
        fseek( file_pointer, 0, SEEK_END );

        const char* delim = " ,\t\n";
        char buffer[2048];
        size_t counter = 0;
        while ( fgets( buffer, sizeof(buffer), file_pointer ) != NULL )
        {
            char* value = strtok( buffer, delim );
            while ( value != NULL )
            {
                if ( counter == index )
                {
                    return kvs::Real64( atof( value ) );
                }

                value = strtok( 0, delim );
                counter++;
            }
        }
    }
    else
    {
        kvsMessageError( "Unknown format '%s'.", format.c_str() );
    }

    kvsMessageError("Cannot read a value in the table object.");

    return 0.0f;
}

template <typename T>
const kvs::AnyValueArray ReadExternalData(
    const size_t index,
    const size_t nvalues,
    const std::string& format,
    FILE* file_pointer )
{
    if ( format == "binary" )
    {
        fseek( file_pointer, sizeof(T) * index, SEEK_SET );

//        kvs::AnyValueArray values;
//        void* pvalues = values.allocate<T>( nvalues );
//        fread( pvalues, sizeof(T), nvalues, file_pointer );
        kvs::ValueArray<T> values( nvalues );
        fread( values.data(), sizeof(T), nvalues, file_pointer );

        return kvs::AnyValueArray( values );
    }
    else if ( format == "ascii" )
    {
        fseek( file_pointer, 0, SEEK_END );

//        kvs::AnyValueArray values;
//        T* pvalues = static_cast<T*>( values.allocate<T>( nvalues ) );
        kvs::ValueArray<T> values( nvalues );
        T* pvalues = values.data();

        const char* delim = " ,\t\n";
        char buffer[2048];
        size_t counter = 0;
        while ( fgets( buffer, sizeof(buffer), file_pointer ) != NULL )
        {
            char* value = strtok( buffer, delim );
            while ( value != NULL )
            {
                if ( counter >= index )
                {
                    const size_t i = counter - index;
                    if ( i < nvalues )
                    {
                        pvalues[i] = kvs::Real64( atof( value ) );
                    }
                    else return kvs::AnyValueArray( values );
                }

                value = strtok( 0, delim );
                counter++;
            }
        }
    }
    else
    {
        kvsMessageError( "Unknown format '%s'.", format.c_str() );
    }

    kvsMessageError("Cannot read a value in the table object.");
    return kvs::AnyValueArray();
}

const size_t GetByteSizePerRow( const kvsoceanvis::pcs::OutOfCoreTableObject* table )
{
    const size_t ncolumns = table->numberOfColumns();
    size_t size = 0;
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        const std::string type = table->columnType( i );
        if ( type == "char" ) size += sizeof(char);
        else if ( type == "unsigned char" || type == "uchar" ) size += sizeof(unsigned char);
        else if ( type == "short" ) size += sizeof(short);
        else if ( type == "unsigned short" || type == "ushort" ) size += sizeof(unsigned short);
        else if ( type == "int" ) size += sizeof(int);
        else if ( type == "unsigned int" || type == "uint" ) size += sizeof(unsigned int);
        else if ( type == "float" ) size += sizeof(float);
        else if ( type == "double" ) size += sizeof(double);
    }

    return size;
}

} // end of namespace


namespace kvsoceanvis
{

namespace pcs
{

OutOfCoreTableObject::OutOfCoreTableObject()
{
    m_cache_enabled = false;
    m_cache_size = 0;
    m_cache_index = 0;
    m_cache_nrows = 0;
}

OutOfCoreTableObject::~OutOfCoreTableObject()
{
    this->closeColumnFiles();
    this->clearCache();
}

void OutOfCoreTableObject::enableCache( const kvs::UInt64 cache_size )
{
    m_cache_enabled = true;
    m_cache_size = cache_size;
    m_cache_nrows = kvs::Math::Min( size_t( m_cache_size ) / ::GetByteSizePerRow( this ), BaseClass::numberOfRows() );
}

void OutOfCoreTableObject::disableCache()
{
    m_cache_enabled = false;
}

void OutOfCoreTableObject::clearCache()
{
    m_cache_size = 0;
    m_cache_index = 0;
    m_cache_nrows = 0;
    m_cache_columns.clear();
}

void OutOfCoreTableObject::fetch() const
{
    m_cache_columns.clear();
    for ( size_t i = 0; i < BaseClass::numberOfColumns(); i++ )
    {
        FILE* file_pointer = m_column_file_pointers[i];
        const std::string type = this->columnType( i );
        const std::string format = this->columnFormat( i );
        if( type == "char" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::Int8>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if( type == "unsigned char" || type == "uchar" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::UInt8>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if ( type == "short" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::Int16>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if ( type == "unsigned short" || type == "ushort" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::UInt16>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if ( type == "int" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::Int32>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if ( type == "unsigned int" || type == "uint" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::UInt32>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if ( type == "float" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::Real32>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
        else if ( type == "double" )
        {
            m_cache_columns.push_back( ::ReadExternalData<kvs::Real64>( m_cache_index, m_cache_nrows, format, file_pointer ) );
        }
    }
}

void OutOfCoreTableObject::openColumnFiles() const
{
    const size_t nfiles = m_column_files.size();
    for ( size_t i = 0; i < nfiles; i++ )
    {
        FILE* fp = NULL;
        if ( m_column_formats[i] == "binary" ) fp = fopen( m_column_files[i].c_str(), "rb" );
        else fp = fopen( m_column_files[i].c_str(), "r" );

        if ( !fp ) kvsMessageError( "Cannot open %s.", m_column_files[i].c_str() );
        m_column_file_pointers.push_back( fp );
    }
}

void OutOfCoreTableObject::closeColumnFiles() const
{
    const size_t nfiles = m_column_file_pointers.size();
    for ( size_t i = 0; i < nfiles; i++ )
    {
        fclose( m_column_file_pointers[i] );
    }

    m_column_file_pointers.clear();
}

const std::string& OutOfCoreTableObject::columnType( const size_t index ) const
{
    return m_column_types[index];
}

const std::string& OutOfCoreTableObject::columnFormat( const size_t index ) const
{
    return m_column_formats[index];
}

const std::string& OutOfCoreTableObject::columnFile( const size_t index ) const
{
    return m_column_files[index];
}

kvs::AnyValueArray OutOfCoreTableObject::column( const size_t index ) const
{
    const std::string type = this->columnType( index );
    const std::string format = this->columnFormat( index );
    const std::string filename = this->columnFile( index );
    const size_t nelements = this->numberOfRows();

    kvs::AnyValueArray values;
    if( type == "char" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Int8>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if( type == "unsigned char" || type == "uchar" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::UInt8>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if ( type == "short" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Int16>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if ( type == "unsigned short" || type == "ushort" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::UInt16>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if ( type == "int" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Int32>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if ( type == "unsigned int" || type == "uint" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::UInt32>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if ( type == "float" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Real32>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else if ( type == "double" )
    {
        if ( !kvs::kvsml::DataArray::ReadExternalData<kvs::Real64>( &values, nelements, filename, format ) )
        {
            kvsMessageError( "Cannot read %s.", filename.c_str() );
        }
    }
    else
    {
        kvsMessageError( "'type' is not specified or unknown data type '%s'.", type.c_str() );
    }

    return values;
}

kvs::ObjectBase::ObjectType OutOfCoreTableObject::objectType() const
{
    return kvs::ObjectBase::UnknownObject;
}

kvs::Real64 OutOfCoreTableObject::readValue( const size_t row_index, const size_t column_index ) const
{
    if ( m_cache_enabled )
    {
        const size_t sindex = m_cache_index;
        const size_t eindex = sindex + m_cache_nrows - 1;
        if ( row_index < sindex || eindex < row_index )
        {
            m_cache_index = row_index;
            m_cache_nrows = kvs::Math::Min( size_t( m_cache_size ) / ::GetByteSizePerRow( this ), BaseClass::numberOfRows() );
            this->fetch();
        }

        return m_cache_columns[ column_index ].at<kvs::Real64>( row_index - m_cache_index );
    }
    else
    {
        FILE* file_pointer = m_column_file_pointers[column_index];
        const std::string type = this->columnType( column_index );
        const std::string format = this->columnFormat( column_index );
        if( type == "char" )
        {
            return ::ReadExternalData<kvs::Int8>( row_index, format, file_pointer );
        }
        else if( type == "unsigned char" || type == "uchar" )
        {
            return ::ReadExternalData<kvs::UInt8>( row_index, format, file_pointer );
        }
        else if ( type == "short" )
        {
            return ::ReadExternalData<kvs::Int16>( row_index, format, file_pointer );
        }
        else if ( type == "unsigned short" || type == "ushort" )
        {
            return ::ReadExternalData<kvs::UInt16>( row_index, format, file_pointer );
        }
        else if ( type == "int" )
        {
            return ::ReadExternalData<kvs::Int32>( row_index, format, file_pointer );
        }
        else if ( type == "unsigned int" || type == "uint" )
        {
            return ::ReadExternalData<kvs::UInt32>( row_index, format, file_pointer );
        }
        else if ( type == "float" )
        {
            return ::ReadExternalData<kvs::Real32>( row_index, format, file_pointer );
        }
        else if ( type == "double" )
        {
            return ::ReadExternalData<kvs::Real64>( row_index, format, file_pointer );
        }

        kvsMessageError( "'type' is not specified or unknown data type '%s'.", type.c_str() );
        return 0.0;
    }
}

} // end of namespace pcs

} // end of namespace kvsoceanvis
