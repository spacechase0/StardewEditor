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
    if ( dragging && !sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
    {
        dragging = false;
    }
}

void Map::update( const sf::Event& event )
{
    if ( event.type == sf::Event::MouseButtonPressed && editor.ui.isMouseOutside() )
    {
        if ( event.mouseButton.button == sf::Mouse::Left )
        {
            switch ( clickMode )
            {
                case Panning:
                    dragging = true;
                    dragFrom = editor.window.mapPixelToCoords( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ) );
                    break;
            }
        }
    }
}

void Map::render( sf::RenderWindow& window )
{
    sf::View view( sf::FloatRect( 0, 0, window.getSize().x, window.getSize().y ) );
    window.setView( view );
    
    if ( dragging )
    {
        sf::Vector2f dragTo = window.mapPixelToCoords( sf::Vector2i( sf::Mouse::getPosition( window ) ) );
        spr.move( dragTo - dragFrom );
        dragFrom = dragTo;
    }
    
    if ( current != "" )
    {
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
    spr.setTexture( tex, true );
    spr.setPosition( 0, 0 );
}

std::string Map::getCurrentMap() const
{
    return current;
}
