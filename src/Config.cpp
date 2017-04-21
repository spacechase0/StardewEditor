#include "Config.hpp"

#include <fstream>
#include <util/String.hpp>

namespace
{
    const char* OPT_UNPACKED_CONTENT = "unpacked_content";
    const char* OPT_DATA = "data";
    const char* OPT_EXTRACTED_SOUNDS = "extracted_sounds";
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
        else if ( opt == OPT_EXTRACTED_SOUNDS )
            extractedSounds = val;
    }
    
    unpackedContent.resize( 512, '\0' );
    dataFolder.resize( 512, '\0' );
    extractedSounds.resize( 512, '\0' );
    
    return true;
}

bool Config::saveToFile( const std::string& path ) const
{
    std::ofstream file( path, std::ofstream::trunc );
    if ( !file )
        return false;
    
    file << util::format( "$=$\n", OPT_UNPACKED_CONTENT, unpackedContent.c_str() );
    file << util::format( "$=$\n", OPT_DATA, dataFolder.c_str() );
    file << util::format( "$=$\n", OPT_EXTRACTED_SOUNDS, extractedSounds.c_str() );
    
    return true;
}

void Config::setUnpackedContentFolder( const std::string& path )
{
    unpackedContent = path;
    unpackedContent.resize( 512, '\0' );
}

std::string Config::getUnpackedContentFolder() const
{
    return unpackedContent.c_str();
}

void Config::setDataFolder( const std::string& path )
{
    dataFolder = path;
    unpackedContent.resize( 512, '\0' );
}

std::string Config::getDataFolder() const
{
    return dataFolder.c_str();
}

void Config::setExtractedSounds( const std::string& theExtractedSounds )
{
    extractedSounds = theExtractedSounds;
    unpackedContent.resize( 512, '\0' );
}

std::string Config::getExtractedSounds() const
{
    return extractedSounds.c_str();
}

void Config::reset()
{
    unpackedContent = "./Content";
    dataFolder = "./data";
    extractedSounds = "./sounds";
    
    unpackedContent.resize( 512, '\0' );
    dataFolder.resize( 512, '\0' );
    extractedSounds.resize( 512, '\0' );
}
