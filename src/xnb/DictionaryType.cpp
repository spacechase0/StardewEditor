#include "xnb/DictionaryType.hpp"

#include <iostream>

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
    
    std::unique_ptr< Data >&& DictionaryTypeReader::read( std::istream& in, const std::string& fullDecl )
    {
        std::size_t startGeneric = fullDecl.find( '`' );
        std::size_t startType1 = startGeneric + 4;
        std::size_t endType1 = fullDecl.find( ',', startGeneric + 3 );
        std::size_t startType2 = fullDecl.find( "],[" ) + 3;
        std::size_t endType2 = fullDecl.find( ',', startType2 );
        
        // tODO: These won't work with generics as is
        std::string type1 = fullDecl.substr( startType1, endType1 - startType1 );
        std::string type2 = fullDecl.substr( startType2, endType2 - startType2 );
        std::unique_ptr< Data > data( new DictionaryData( type1, type2 ) );
        DictionaryData* dict = static_cast< DictionaryData* >( data.get() );
        
        sf::Uint32 count;
        in.read( reinterpret_cast< char* >( &count ), sizeof( count ) );
        std::cout << type1 << ' ' << type2 << ' ' << count << ' ' << (int)in.get() << std::endl;
        for ( std::size_t i = 0; i < count; ++i )
        {
            auto key = ITypeReader::getTypeReader( type1 )->read( in, type1 );
            auto value = ITypeReader::getTypeReader( type2 )->read( in, type2 );
            dict->data.emplace( std::move( key ), std::move( value ) );
        }
        std::cout<<"Y:"<<data.get()<<' ';
        auto x=std::move(data);
        std::cout<<x.get()<<std::endl;
        return std::move( data );
    }
}
