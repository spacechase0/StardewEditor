#include "xnb/ITypeReader.hpp"

#include <SFML/Config.hpp>

#include "xnb/ArrayType.hpp"
#include "xnb/DictionaryType.hpp"
#include "xnb/File.hpp"
#include "xnb/ListType.hpp"
#include "xnb/PrimitiveType.hpp"
#include "xnb/SpriteFontType.hpp"
#include "xnb/StringType.hpp"
#include "xnb/TbinType.hpp"
#include "xnb/TextureType.hpp"

#include"IOSTREAM"

namespace xnb
{
    bool operator == ( const TypeReaderHeader& a, const TypeReaderHeader& b )
    {
        return ( a.name == b.name && a.version == b.version );
    }
    
    ITypeReader::~ITypeReader()
    {
    }
    
    ITypeReader* ITypeReader::getTypeReader( std::string str )
    {
        std::size_t genericMarker = str.find( '`' );
        if ( genericMarker != std::string::npos )
            str = str.substr( 0, genericMarker );
        
        std::size_t infoSep = str.find( ',' );
        if ( infoSep != std::string::npos )
            str = str.substr( 0, infoSep );
        
        static std::unique_ptr< ITypeReader > arrays;
        static std::map< std::string, std::unique_ptr< ITypeReader > > types;
        static std::map< std::string, std::unique_ptr< ITypeReader > > typeReaders;
        if ( types.size() == 0 )
        {
            arrays.reset( new ArrayTypeReader() );
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
            ADD_TYPEREADER( TEXTURE2D_TYPE, new Texture2DTypeReader() );
            ADD_TYPEREADER( TBIN_TYPE, new TbinTypeReader() );
            //ADD_TYPEREADER( LIST_TYPE, new ListTypeReader() );
            //ADD_TYPEREADER( SPRITEFONT_TYPE, new SpriteFontTypeReader() );
            
            #undef ADD_TYPEREADER
        }
        
        if ( ( str[ str.length() - 2 ] == '[' && str[ str.length() - 1 ] == ']' ) || str == "Microsoft.Xna.Framework.Content.ArrayReader" )
        {
            return arrays.get();
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
    
    std::string ITypeReader::getTypeReaderNameAtIndex( const File& file, int index )
    {
        return file.typeReaders[ index ].name;
    }
    
    int ITypeReader::getIndexOfTypeReader( const File& file, const std::string& name )
    {
        for ( std::size_t i = 0; i < file.typeReaders.size(); ++i )
        {
            if ( file.typeReaders[ i ].name == name )
                return i;
        }
        return -1;
    }
}
