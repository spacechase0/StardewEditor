#include "xnb/File.hpp"

#include <array>
#include <fstream>
#include <sstream>
#include <SFML/Config.hpp>
#include <util/String.hpp>

#include "xnb/ITypeReader.hpp"
#include "xnb/Util.hpp"

namespace xnb
{
    // TODO: Move
    enum TargetPlatform
    {
        Windows = 'w',
        Phone = 'm',
        Xbox = 'x',
    };
    enum Flags
    {
        HiDef = 0x01,
        Compressed = 0x80,
    };
    
    template< typename T >
    T read( std::istream& in )
    {
        T t;
        in.read( reinterpret_cast< char* >( &t ), sizeof( T ) );
        return t;
    }
    
    /// NOTE: UTF-8. Change to sf::String?
    template<>
    std::string read< std::string >( std::istream& in )
    {
        auto bytes = read7BitEncodedInt( in );
        std::string str( bytes, 0 );
        in.read( &str[ 0 ], bytes );
        return str;
    }
    
    struct TypeReaderHeader
    {
        std::string name;
        sf::Int32 version;
    };
    
    template<>
    TypeReaderHeader read< TypeReaderHeader >( std::istream& in )
    {
        TypeReaderHeader tr;
        tr.name = read< std::string >( in );
        tr.version = read< sf::Int32 >( in );
        return tr;
    }
    
    bool File::loadFromFile( const std::string& path )
    {
        std::ifstream file( path, std::ifstream::binary );
        if ( !file )
        {
            util::log( "[ERROR] $: Failed to open file.\n", path );
            return false;
        }
        
        util::log( "[INFO] Reading $...\n", path );
        return loadFromStream( file );
    }
    
    bool File::loadFromStream( std::istream& in )
    {
        in.exceptions( std::ifstream::failbit );
        
        try
        {
            if ( in.get() != 'X' || in.get() != 'N' || in.get() != 'B' )
            {
                util::log( "[ERROR] File is not an XNB file.\n" );
                return false;
            }
            if ( in.get() != TargetPlatform::Windows )
            {
                util::log( "[WARN] Other target platforms untested.\n" );
            }
            if ( in.get() != 5 )
            {
                util::log( "[ERROR] Format version unsupported.\n" );
                return false;
            }
            
            char flags = in.get();
            bool hidef = flags & HiDef;
            bool compressed = flags & Compressed;
            
            auto sizeCompressed = read< sf::Uint32 >( in );
            auto sizeDecompressed = compressed ? read< sf::Uint32 >( in ) : sizeCompressed;
            
            if ( compressed )
            {
                throw std::invalid_argument( "Decompression not implemented." );
                /*
                std::istringstream ins( decompress( in, sizeCompressed - 14, sizeDecompressed ) );
                ins.exceptions( std::ifstream::failbit );
                return readData( ins );
                */
            }
            else
            {
                return readData( in );
            }
        }
        catch ( std::exception& e )
        {
            util::log( "[ERROR] Exception reading: $\n", e.what() );
            return false;
        }
    }
    
    bool File::readData( std::istream& in )
    {
        std::vector< TypeReaderHeader > trs;
        trs.resize( read7BitEncodedInt( in ) );
        for ( TypeReaderHeader& tr : trs )
        {
            tr = read< TypeReaderHeader >( in );
        }
        
        int sharedResCount = read7BitEncodedInt( in );
        
        int type = read7BitEncodedInt( in );
        if ( type == 0 )
        {
            throw std::invalid_argument( util::format( "Null not supported" ) );
        }
        else
        {
            type -= 1;
            std::string typeStr = trs[ type ].name;
            std::size_t genericMarker = typeStr.find( '`' );
            if ( genericMarker != std::string::npos )
                typeStr = typeStr.substr( 0, genericMarker );
            
            ITypeReader* reader = ITypeReader::getTypeReader( typeStr );
            if ( reader == nullptr )
                throw std::invalid_argument( util::format( "Type not supported: $", typeStr ) );
            std::cout<<typeStr<<std::endl;
            data = reader->read( in, trs[ type ].name );
            std::cout<<data.get()<<std::endl;
        }
        
        return true;
    }
}
