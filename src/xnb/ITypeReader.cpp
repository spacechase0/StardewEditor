#include "xnb/ITypeReader.hpp"

#include <SFML/Config.hpp>

#include "xnb/DictionaryType.hpp"
#include "xnb/PrimitiveType.hpp"
#include "xnb/StringType.hpp"

namespace xnb
{
    ITypeReader::~ITypeReader()
    {
    }
    
    ITypeReader* ITypeReader::getTypeReader( const std::string& str )
    {
        static std::map< std::string, std::unique_ptr< ITypeReader > > types;
        static std::map< std::string, std::unique_ptr< ITypeReader > > typeReaders;
        if ( types.size() == 0 )
        {
            #define ADD_TYPEREADER(str,r) types.emplace( std::make_pair( str, std::unique_ptr< ITypeReader >() ) ); \
                                          types[ str ].reset( r ); \
                                          typeReaders.emplace( std::make_pair( str ## READER, std::unique_ptr< ITypeReader >() ) ); \
                                          typeReaders[ str ## READER ].reset( r );
            
            ADD_TYPEREADER( PRIMITIVE_INT8_TYPE, new PrimitiveTypeReader< sf::Int8 >() );
            ADD_TYPEREADER( PRIMITIVE_UINT8_TYPE, new PrimitiveTypeReader< sf::Uint8 >() );
            ADD_TYPEREADER( PRIMITIVE_INT16_TYPE, new PrimitiveTypeReader< sf::Int16 >() );
            ADD_TYPEREADER( PRIMITIVE_UINT16_TYPE, new PrimitiveTypeReader< sf::Uint16 >() );
            ADD_TYPEREADER( PRIMITIVE_INT32_TYPE, new PrimitiveTypeReader< sf::Int32 >() );
            ADD_TYPEREADER( PRIMITIVE_UINT32_TYPE, new PrimitiveTypeReader< sf::Uint32 >() );
            ADD_TYPEREADER( PRIMITIVE_INT64_TYPE, new PrimitiveTypeReader< sf::Int64 >() );
            ADD_TYPEREADER( PRIMITIVE_UINT64_TYPE, new PrimitiveTypeReader< sf::Uint64 >() );
            ADD_TYPEREADER( PRIMITIVE_FLOAT_TYPE, new PrimitiveTypeReader< float >() );
            ADD_TYPEREADER( PRIMITIVE_DOUBLE_TYPE, new PrimitiveTypeReader< double >() );
            ADD_TYPEREADER( PRIMITIVE_BOOL_TYPE, new PrimitiveTypeReader< bool >() );
            ADD_TYPEREADER( DICTIONARY_TYPE, new DictionaryTypeReader() );
            ADD_TYPEREADER( STRING_TYPE, new StringTypeReader() );
            
            #undef ADD_TYPEREADER
        }
        
        auto it = typeReaders.find( str );
        if ( it != typeReaders.end() )
        {
            return it->second.get();
        }
        it = types.find( str );
        if ( it != types.end() )
        {
            return it->second.get();
        }
        return nullptr;
    }
}
