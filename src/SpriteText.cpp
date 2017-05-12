#include "SpriteText.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "SpriteFont.hpp"

SpriteText::SpriteText()
{
}

SpriteText::SpriteText( const sf::String& theStr, const SpriteFont& theFont )
:   str( theStr ),
    font( &theFont )
{
}

void SpriteText::setString( const sf::String& theStr )
{
    str = theStr;
    dirty = true;
}

void SpriteText::setFont( const SpriteFont& theFont )
{
    font = &theFont;
    dirty = true;
}

void SpriteText::setColor( const sf::Color& theColor )
{
    color = theColor;
    dirty = true;
}

const sf::String& SpriteText::getString() const
{
    return str;
}

const SpriteFont* SpriteText::getFont() const
{
    return font;
}

const sf::Color& SpriteText::getColor() const
{
    return color;
}

sf::FloatRect SpriteText::getLocalBounds() const
{
    return bounds;
}

sf::FloatRect SpriteText::getGlobalBounds() const
{
    return getTransform().transformRect( getLocalBounds() );
}

void SpriteText::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    if ( dirty )
        build();
    
    states.transform *= getTransform();
    states.texture = &font->getTexture();
    target.draw( &vertices[ 0 ], vertices.size(), sf::PrimitiveType::Quads, states );
}

void SpriteText::build() const
{
    if ( font == nullptr || !dirty )
        return;
    
    vertices.clear();
    bounds = sf::FloatRect( 0, 0, 0, 0);
    
    sf::Vector2i spacing = font->getSpacing();
    
    sf::Vector2f pos = -getOrigin();
    bounds.left = pos.x;
    bounds.top = pos.y;
    
    for ( auto c : str.toAnsiString() )
    {
        if ( c == '\r' )
            continue;
        
        if ( c == '\n' )
        {
            bounds.width = std::max( bounds.width, pos.x );
            pos.x = 0;
            pos.y += spacing.y;
        }
        else
        {
            sf::IntRect glyph = font->getGlyph( c );
            sf::IntRect crop = font->getCropping( c );
            sf::Vector3i kern = font->getKerning( c );
            
            sf::Vector2f offset( kern.x, kern.y );
            offset.x += spacing.x;
            offset.x += crop.left;
            offset.y += crop.top;
            
            vertices.push_back( sf::Vertex( pos + offset + sf::Vector2f( 0         , 0           ), color, sf::Vector2f( glyph.left              , glyph.top                ) ) );
            vertices.push_back( sf::Vertex( pos + offset + sf::Vector2f( crop.width, 0           ), color, sf::Vector2f( glyph.left + glyph.width, glyph.top                ) ) );
            vertices.push_back( sf::Vertex( pos + offset + sf::Vector2f( crop.width, crop.height ), color, sf::Vector2f( glyph.left + glyph.width, glyph.top + glyph.height ) ) );
            vertices.push_back( sf::Vertex( pos + offset + sf::Vector2f( 0         , crop.height ), color, sf::Vector2f( glyph.left              , glyph.top + glyph.height ) ) );
            
            pos.x += kern.y + kern.z;
        }
    }
    
    bounds.height = pos.y;
    
    dirty = false;
}
