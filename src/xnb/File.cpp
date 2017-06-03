#include "xnb/File.hpp"

#include <array>
#include <fstream>
#include <sstream>
#include <SFML/Config.hpp>

extern "C" 
{
    #include "mspack.h"
    #include "system.h"
    #include "lzx.h"
}

namespace
{
    struct MyFile : mspack_file
    {
        std::istream* const in = nullptr;
        std::ostream* const out =  nullptr;
        
        MyFile( std::istream* const stream )
        :   in( stream )
        {
        }
        
        MyFile( std::ostream* const stream )
        :   out( stream )
        {
        }
        
        MyFile( std::iostream* const stream )
        :   in( stream ), out( stream )
        {
        }
    };
    
    int myRead( mspack_file* file_, void* buffer, int bytes )
    {
        auto file = static_cast< MyFile* >( file_ );
        file->in->read( static_cast< char* >( buffer ), bytes );
        if ( file->in->eof() )
            return 0;
        if ( !( * file->in ) )
            return -1;
        return file->in->gcount();
    }
    
    int myWrite( mspack_file* file_, void* buffer, int bytes )
    {
        auto file = static_cast< MyFile* >( file_ );
        file->out->write( static_cast< char* >( buffer ), bytes );
        if ( !( * file->out ) )
            return -1;
        return bytes;
    }
    
    void* myAlloc( mspack_system* self, size_t bytes )
    {
        return std::malloc( bytes );
    }
    
    void myFree( void* ptr )
    {
        std::free( ptr );
    }
    
    void myCopy( void* src, void* dest, std::size_t bytes )
    {
        // error: 'memcpy' is not a member of 'std'|
        //  suggested alternative: 'memcpy'
        // What?
        memcpy( src, dest, bytes );
    }
    
    struct mspack_system mySystem =
    {
        nullptr, // open, not needed
        nullptr, // close, not needed
        myRead,
        myWrite,
        nullptr, // seek, not needed
        nullptr, // tell, not needed
        nullptr, // message, not needed
        myAlloc,
        myFree,
        myCopy,
        nullptr // Just a null pointer
    };
    
    std::string decompress( std::istream& in, int compSize, int decompSize )
    {
        std::ostringstream out;
        MyFile inf( &in ), outf( &out );
        
        std::cout << "Starting decompression" << std::endl;
        {
            char a = in.get(), b = in.get();
            int chunk = 0x8000;
            int block = ( a << 8 ) | b;
            if ( a == 0xFF )
            {
                chunk = ( a << 8 ) | in.get();
                block = ( in.get() << 8 ) | in.get();
            }
            
            auto stream = lzxd_init( &mySystem, &inf, &outf, 16, 0, block, chunk, false );
            int err = lzxd_decompress( stream, chunk );
            std::cout << "Finished: " << err << ' ' << stream->error << std::endl;
            lzxd_free( stream ); 
        }
        
        std::string ret = out.str();
        std::cout << in.tellg() << ' ' << compSize << " decompressed: " << ret.length() << ' ' << ret << std::endl;
        
        return out.str();
    }
}

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
    
    // From http://xbox.create.msdn.com/en-US/sample/xnb_format
    sf::Int32 read7BitEncodedInt( std::istream& in )
    {
        int ret = 0;
        int bitsRead = 0;
        int value;
        
        do
        {
            value = in.get();
            ret |= ( value & 0x7F ) << bitsRead;
            bitsRead += 7;
        }
        while ( value & 0x80 );
        
        return ret;
    }
    
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
    
    struct TypeReader
    {
        std::string name;
        sf::Int32 version;
    };
    
    template<>
    TypeReader read< TypeReader >( std::istream& in )
    {
        TypeReader tr;
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
                std::istringstream ins( decompress( in, sizeCompressed - 14, sizeDecompressed ) );
                ins.exceptions( std::ifstream::failbit );
                return readData( ins );
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
        std::vector< TypeReader > trs;
        trs.resize( read7BitEncodedInt( in ) );
        util::log("trc:$\n",trs.size());
        for ( TypeReader& tr : trs )
        {
            tr = read< TypeReader >( in );
            util::log("tr:$ $\n",tr.name,tr.version);
        }
        
        return true;
    }
}
