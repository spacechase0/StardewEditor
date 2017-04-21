#include "Actor.hpp"

#include <boost/filesystem.hpp>

#include "Config.hpp"
#include "Editor.hpp"

Actor::Actor( Editor& theEditor, const std::string& theName, sf::Vector2i thePos, int theFacing )
:   editor( theEditor ),
    name( theName ),
    tilePos( thePos ),
    facing( theFacing )
{
}

std::string Actor::getName() const
{
    return name;
}

void Actor::setPosition( sf::Vector2i thePos )
{
    tilePos = thePos;
    spr.setPosition( tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE );
}

sf::Vector2i Actor::getPosition() const
{
    return tilePos;
}

void Actor::setFacing( int theFacing )
{
    facing = theFacing % 4;
    spr.setTextureRect( sf::IntRect( 0, facing * 32, 16, 32 ) );
}

int Actor::getFacing() const
{
    return facing;
}

void Actor::render( sf::RenderWindow& window )
{
    window.draw( spr );
}

void Actor::init()
{
    fs::path path( editor.config.getUnpackedContentFolder() );
    path /= "Characters";
    if ( name.length() >= 6 && name.substr( 0, 6 ) == "farmer" )
        path = path / "Farmer" / "farmer_base.png";
    else path /= name + ".png";
    
    tex.loadFromFile( path.string() );
    spr.setTexture( tex );
    spr.setPosition( tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE );
    spr.setTextureRect( sf::IntRect( 0, facing * 32, 16, 32 ) );
    spr.setOrigin( 0, 16 );
}
