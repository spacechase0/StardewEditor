#include "Config.hpp"

#include <fstream>
#include <util/String.hpp>

namespace
{
    const char* OPT_UNPACKED_CONTENT = "unpacked_content";
    const char* OPT_DATA = "data";
}

Config::Config()
{
    reset();
}

bool Config::loadFromFile( const std::string& path )
{
    std::ifstream file( path );
    if ( !file )
        return false;
    
    Config old = ( * this );
    reset();
    
    while ( true )
    {
        std::string line;
        std::getline( file, line );
        if ( !file )
            break;
        
        std::size_t eq = line.find( '=' );
        if ( eq == std::string::npos)
            continue;
        
        std::string opt = line.substr( 0, eq );
        std::string val = line.substr( eq + 1 );
        
        if ( opt == OPT_UNPACKED_CONTENT )
            unpackedContent = val;
        else if ( opt == OPT_DATA )
            dataFolder = val;
    }
    
    return true;
}

bool Config::saveToFile( const std::string& path ) const
{
    std::ofstream file( path, std::ofstream::trunc );
    if ( !file )
        return false;
    
    file << util::format( "$=$\n", OPT_UNPACKED_CONTENT, unpackedContent );
    file << util::format( "$=$\n", OPT_DATA, dataFolder );
    
    return true;
}

void Config::setUnpackedContentFolder( const std::string& path )
{
    unpackedContent = path;
}

std::string Config::getUnpackedContentFolder() const
{
    return unpackedContent;
}

void Config::setDataFolder( const std::string& path )
{
    dataFolder = path;
}

std::string Config::getDataFolder() const
{
    return dataFolder;
}

void Config::reset()
{
    unpackedContent = "./Content";
    dataFolder = "./data";
}
