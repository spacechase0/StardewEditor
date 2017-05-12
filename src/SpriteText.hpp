#ifndef SPRITETEXT_HPP
#define SPRITETEXT_HPP

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace sf
{
    class RenderTarget;
}

class SpriteFont;

class SpriteText : public sf::Drawable, public sf::Transformable
{
    public:
        SpriteText();
        SpriteText( const sf::String& str, const SpriteFont& font );
        
        void setString( const sf::String& str );
        void setFont( const SpriteFont& font );
        void setColor( const sf::Color& color );
        const sf::String& getString() const;
        const SpriteFont* getFont() const;
        const sf::Color& getColor() const;
        
        sf::FloatRect getLocalBounds() const;
        sf::FloatRect getGlobalBounds() const;
        
        virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;
    
    private:
        sf::String str = "";
        const SpriteFont* font = nullptr;
        sf::Color color = sf::Color( 34, 17, 34 );
        
        mutable bool dirty = true;
        mutable std::vector< sf::Vertex > vertices;
        mutable sf::FloatRect bounds;
        void build() const;
};

#endif // SPRITETEXT_HPP
