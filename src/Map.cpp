#include "Map.hpp"

#include <boost/filesystem.hpp>
#include <SFML/Graphics.hpp>

#include "Editor.hpp"

Map::Map( Editor& theEditor )
:   editor( theEditor )
{
}

void Map::update()
{
}

void Map::update( const sf::Event& event )
{
}

void Map::render( sf::RenderWindow& window )
{
    sf::View view( sf::FloatRect( 0, 0, window.getSize().x, window.getSize().y ) );
    window.setView( view );
    
    if ( current != "" )
    {
        sf::Sprite spr( tex );
        window.draw( spr );
    }
}

void Map::changeCurrentMap( const std::string& map )
{
    if ( !tex.loadFromFile( ( fs::path( editor.config.getMapImagesFolder() ) / ( map + ".png" ) ).string() ) )
    {
        std::cout << "Error: Couldn't load map image\n";
        return;
    }
    current = map;
}

std::string Map::getCurrentMap() const
{
    return current;
}
