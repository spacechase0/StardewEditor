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
    if ( firstUpdate )
    {
        view = sf::View( sf::FloatRect( 0, 0, editor.window.getSize().x, editor.window.getSize().y ) );
        
        firstUpdate = false;
    }
    
    if ( dragging && !sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
    {
        dragging = false;
    }
}

void Map::update( const sf::Event& event )
{
    if ( event.type == sf::Event::EventType::MouseButtonPressed && editor.ui.isMouseOutside() )
    {
        if ( event.mouseButton.button == sf::Mouse::Left )
        {
            switch ( clickMode )
            {
                case Panning:
                    dragging = true;
                    dragFrom = pixelToWorld( sf::Vector2i( event.mouseButton.x, event.mouseButton.y ) );
                    break;
            }
        }
    }
    else if ( event.type == sf::Event::EventType::MouseWheelScrolled && editor.ui.isMouseOutside() )
    {
        sf::Vector2f worldPos = editor.window.mapPixelToCoords( sf::Vector2i( event.mouseWheelScroll.x, event.mouseWheelScroll.y ), view );
         
        if ( event.mouseWheelScroll.delta < 0 )
            view.zoom( 1.1 );
        else
            view.zoom( 0.9 );
        
        sf::Vector2f newWorldPos = editor.window.mapPixelToCoords( sf::Vector2i( event.mouseWheelScroll.x, event.mouseWheelScroll.y ), view );
        view.move( worldPos - newWorldPos );
    }
    else if ( event.type == sf::Event::EventType::Resized )
    {
        sf::Vector2f oldCenter = view.getCenter();
        view = sf::View( sf::FloatRect( 0, 0, editor.window.getSize().x, editor.window.getSize().y ) );
        view.setCenter( oldCenter );
    }
}

void Map::render( sf::RenderWindow& window )
{
    window.setView( view );
    
    if ( dragging )
    {
        sf::Vector2i mouse = sf::Vector2i( sf::Mouse::getPosition( window ) );
        sf::Vector2f dragTo = pixelToWorld( mouse );
        view.move( dragFrom - dragTo );
        dragFrom = pixelToWorld( mouse );
    }
    
    if ( current != "" )
    {
        window.draw( spr );
    }
}

void Map::changeCurrentMap( const std::string& map )
{
    if ( !tex.loadFromFile( ( fs::path( editor.config.getDataFolder() ) / "maps" / ( map + ".png" ) ).string() ) )
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

sf::Vector2f Map::pixelToWorld( sf::Vector2i pixel ) const
{
    return editor.window.mapPixelToCoords( pixel, view );
}
