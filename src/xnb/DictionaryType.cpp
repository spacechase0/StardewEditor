#include "xnb/DictionaryType.hpp"

#include "xnb/Util.hpp"

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
    
    std::unique_ptr< Data > DictionaryTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        std::size_t startGeneric = fullDecl.find( '`' );
        std::size_t startType1 = startGeneric + 4;
        std::size_t endType1 = fullDecl.find( ',', startGeneric + 4 );
        std::size_t startType2 = fullDecl.find( "],[" ) + 3;
        std::size_t endType2 = fullDecl.find( ',', startType2 );
        
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
    
    bool DictionaryTypeReader::resultIsValueType() const
    {
        return false;
    }
}
