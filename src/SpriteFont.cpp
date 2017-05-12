#include "SpriteFont.hpp"

#include <boost/filesystem.hpp>
#include <util/File.hpp>

bool SpriteFont::loadFromFile( const std::string& path )
{
    if ( !tex.loadFromFile( path + ".texture.png" ) )
        return false;
    
    glyphs.clear();
    cropping.clear();
    characters.clear();
    spacing = sf::Vector2i( 0, 0 );
    kerning.clear();
    
    std::string contents = util::getFileContents( path + ".yaml", true );
    std::size_t glyphPos = contents.find( "glyphs:" );
    std::size_t cropPos = contents.find( "cropping:" );
    std::size_t charPos = contents.find( "characterMap:" );
    std::size_t vertPos = contents.find( "verticalSpacing:" );
    std::size_t horPos = contents.find( "horizontalSpacing:" );
    std::size_t kernPos = contents.find( "kerning:" );
    std::size_t defPos = contents.find( "defaultCharacter:" );
    std::size_t endPos = contents.find( "extractedImages:", defPos );
    
    if ( glyphPos == std::string::npos || cropPos == std::string::npos || charPos == std::string::npos ||
         vertPos  == std::string::npos || horPos  == std::string::npos || kernPos == std::string::npos ||
         defPos   == std::string::npos || endPos  == std::string::npos )
    {
        return false;
    }
    
    for ( std::size_t x = contents.find( "x:", glyphPos ); x < cropPos; x = contents.find( "x:", x + 1 ) )
    {
        std::size_t y = contents.find( "y:", x );
        std::size_t w = contents.find( "width:", x );
        std::size_t h = contents.find( "height:", y );
        
        std::string xs = contents.substr( x + 3, contents.find( '\n', x ) - x - 3 );
        std::string ys = contents.substr( y + 3, contents.find( '\n', y ) - y - 3 );
        std::string ws = contents.substr( w + 7, contents.find( '\n', w ) - w - 7 );
        std::string hs = contents.substr( h + 8, contents.find( '\n', h ) - h - 8 );
        
        glyphs.push_back( sf::IntRect( util::fromString< int >( xs ), util::fromString< int >( ys ), util::fromString< int >( ws ), util::fromString< int >( hs ) ) );
    }
    
    for ( std::size_t x = contents.find( "x:", cropPos ); x < charPos; x = contents.find( "x:", x + 1 ) )
    {
        std::size_t y = contents.find( "y:", x );
        std::size_t w = contents.find( "width:", x );
        std::size_t h = contents.find( "height:", y );
        
        std::string xs = contents.substr( x + 3, contents.find( '\n', x ) - x - 3 );
        std::string ys = contents.substr( y + 3, contents.find( '\n', y ) - y - 3 );
        std::string ws = contents.substr( w + 7, contents.find( '\n', w ) - w - 7 );
        std::string hs = contents.substr( h + 8, contents.find( '\n', h ) - h - 8 );
        
        cropping.push_back( sf::IntRect( util::fromString< int >( xs ), util::fromString< int >( ys ), util::fromString< int >( ws ), util::fromString< int >( hs ) ) );
    }
    
    for ( std::size_t i = contents.find( "- \"", charPos); i < vertPos; i = contents.find( "- \"", i + 1 ) )
    {
        char c = contents[ i + 3 ];
        if ( c == '\\' )
            c = contents[ i + 4 ];
        
        characters.push_back( c );
    }
    
    spacing.y = util::fromString< int >( contents.substr( vertPos + 17, contents.find( '\n', vertPos ) - vertPos - 17 ) );
    spacing.x = util::fromString< int >( contents.substr( horPos + 19, contents.find( '\n', horPos ) - horPos - 19 ) );
    
    for ( std::size_t x = contents.find( "x:", cropPos ); x < charPos; x = contents.find( "x:", x + 1 ) )
    {
        std::size_t y = contents.find( "y:", x );
        std::size_t z = contents.find( "z:", x );
        
        std::string xs = contents.substr( x + 3, contents.find( '\n', x ) - x - 3 );
        std::string ys = contents.substr( y + 3, contents.find( '\n', y ) - y - 3 );
        std::string zs = contents.substr( z + 3, contents.find( '\n', z ) - z - 3 );
        
        kerning.push_back( sf::Vector3i( util::fromString< int >( xs ), util::fromString< int >( ys ), util::fromString< int >( zs ) ) );
    }
    
    defaultChar = contents[ contents.find( '"', defPos ) + 1 ];
    
    /*
    util::log( "\nglyphs: " );
    for ( auto x : glyphs ) util::log( "($,$,$,$) ", x.left, x.top, x.width, x.height );
    util::log( "\ncropping: " );
    for ( auto x : cropping ) util::log( "($,$,$,$) ", x.left, x.top, x.width, x.height );
    util::log( "\ncharacters: " );
    for ( auto x : characters ) util::log( "$", x );
    util::log( "\nspacing: ($, $)", spacing.x, spacing.y );
    util::log( "\nkerning: " );
    for ( auto x : kerning ) util::log( "($,$,$) ", x.x, x.y, x.z );
    util::log( "\ndefault char: $", defaultChar );
    //*/
    
    return true;
}
