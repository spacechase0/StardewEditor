#include "xnb/DictionaryType.hpp"

#include "xnb/Util.hpp"
#include                  "IOSTREAM"
namespace xnb
{
    DictionaryData::DictionaryData( const std::string& key, const std::string& value )
    :   keyType( key ), valueType( value )
    {
    }
    
    std::string DictionaryData::toString() const
    {
        std::string str = "Dictionary{";
        for ( auto it = data.begin(); it != data.end(); ++it )
        {
            str += it->first->toString() + '=' + it->second->toString() + ',';
        }
        str[ str.length() - 1 ] = '}';
        return str;
    }
    
    std::vector< TypeReaderHeader > DictionaryData::getTypeReaders() const
    {
        std::vector< TypeReaderHeader > ret;
        ret.push_back( getTypeReader() );
        ret.push_back( TypeReaderHeader{ ITypeReader::getTypeReader( keyType )->getReaderType() } );
        ret.push_back( TypeReaderHeader{ ITypeReader::getTypeReader( valueType )->getReaderType() } );
        
        for ( auto itd = data.begin(); itd != data.end(); ++itd )
        {
            std::vector< TypeReaderHeader > trhs = itd->first->getTypeReaders();
            for ( std::size_t itr = 0; itr < trhs.size(); ++itr )
            {
                bool dup = false;
                for ( std::size_t ir = 0; ir < ret.size(); ++ir )
                {
                    if ( ret[ ir ] == trhs[ itr ] )
                    {
                        dup = true;
                        break;
                    }
                }
                
                if ( !dup )
                {
                    ret.push_back( trhs[ itr ] );
                }
            }
            
            trhs = itd->second->getTypeReaders();
            for ( std::size_t itr = 0; itr < trhs.size(); ++itr )
            {
                bool dup = false;
                for ( std::size_t ir = 0; ir < ret.size(); ++ir )
                {
                    if ( ret[ ir ] == trhs[ itr ] )
                    {
                        dup = true;
                        break;
                    }
                }
                
                if ( !dup )
                {
                    ret.push_back( trhs[ itr ] );
                }
            }
        }
        
        return std::move( ret );
    }
    
    std::string DictionaryData::getType() const
    {
        return DICTIONARY_TYPE;
    }
    
    TypeReaderHeader DictionaryData::getTypeReader() const
    {
        return TypeReaderHeader{ std::string( DICTIONARY_TYPEREADER ) + "`2[[" + keyType + "],[" + valueType + "]]", 0 };
    }
    
    std::unique_ptr< Data > DictionaryTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::size_t startGeneric = fullDecl.find( '`' );
        std::size_t startType1 = startGeneric + 4;
        std::size_t endType1, startType2, endType2;
        startType2 = fullDecl.find( "],[" ) + 3;
        if ( fullDecl.find( ',', startType1 ) == fullDecl.find( ']', startType1 ) + 1 )
        {
            std::cout<<"A";
            // My files don't have the full named with assembly info
            endType1 = fullDecl.find( ']', startType1 );
            endType2 = fullDecl.find( ']', startType2 + 3 );
        }
        else
        {
            std::cout<<"B";
            endType1 = fullDecl.find( ',', startGeneric + 4 );
            endType2 = fullDecl.find( ',', startType2 );
        }
        
        // tODO: These won't work with generics as is
        std::string type1 = fullDecl.substr( startType1, endType1 - startType1 );
        std::string type2 = fullDecl.substr( startType2, endType2 - startType2 );
        std::unique_ptr< Data > data( new DictionaryData( type1, type2 ) );
        DictionaryData* dict = static_cast< DictionaryData* >( data.get() );
        
        sf::Uint32 count;
        in.read( reinterpret_cast< char* >( &count ), sizeof( count ) );
        for ( std::size_t i = 0; i < count; ++i )
        {
            std::unique_ptr< Data > key;
            auto keyReader = ITypeReader::getTypeReader( type1 );
            if ( !keyReader->resultIsValueType() )
            {
                int properReader = read7BitEncodedInt( in );
                if ( properReader != 0 )
                {
                    keyReader = ITypeReader::getTypeReader( getTypeReaderNameAtIndex( file, properReader - 1 ) );
                    key = keyReader->read( file, in, type1 );
                }
            }
            else key = keyReader->read( file, in, type1 );
            
            std::unique_ptr< Data > value;
            auto valueReader = ITypeReader::getTypeReader( type2 );
            if ( !valueReader->resultIsValueType() )
            {
                int properReader = read7BitEncodedInt( in );
                if ( properReader != 0 )
                {
                    valueReader = ITypeReader::getTypeReader( getTypeReaderNameAtIndex( file, properReader - 1 ) );
                    value = valueReader->read( file, in, type2 );
                }
            }
            else value = valueReader->read( file, in, type2 );
            
            dict->data.emplace( std::move( key ), std::move( value ) );
        }
        
        return std::move( data );
    }
    
    void DictionaryTypeReader::write( const File& file, std::ostream& out, const Data* data )
    {
        const DictionaryData* dict = dynamic_cast< const DictionaryData* >( data );
        if ( dict == nullptr )
            throw std::invalid_argument( "Bad data type passed to write" );
        
        sf::Uint32 count = dict->data.size();
        out.write( reinterpret_cast< const char* >( &count ), 4 );
        for ( const auto& dataVal : dict->data )
        {
            ITypeReader* reader1 = ITypeReader::getTypeReader( dataVal.first->getType() );
            ITypeReader* reader2 = ITypeReader::getTypeReader( dataVal.second->getType() );
            if ( reader1 == nullptr )
                throw std::invalid_argument( util::format( "Not supported: $", dataVal.first->getType() ) );
            if ( reader2 == nullptr )
                throw std::invalid_argument( util::format( "Not supported: $", dataVal.second->getType() ) );
            
            if ( !reader1->resultIsValueType() )
            {
                write7BitEncodedInt( out, getIndexOfTypeReader( file, dataVal.first->getTypeReader().name ) + 1 );
                reader1->write( file, out, dataVal.first.get() );
            }
            else reader1->write( file, out, dataVal.first.get() );
            
            if ( !reader2->resultIsValueType() )
            {
                write7BitEncodedInt( out, getIndexOfTypeReader( file, dataVal.second->getTypeReader().name ) + 1 );
                reader2->write( file, out, dataVal.second.get() );
            }
            else reader2->write( file, out, dataVal.second.get() );
        }
    }
    
    bool DictionaryTypeReader::resultIsValueType() const
    {
        return false;
    }
    
    std::string DictionaryTypeReader::getReaderType() const
    {
        return DICTIONARY_TYPEREADER;
    }
}
