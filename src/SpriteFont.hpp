#ifndef SPRITEFONT_HPP
#define SPRITEFONT_HPP

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

class SpriteFont
{
    public:
        bool loadFromFile( const std::string& path );
        
        const sf::Texture& getTexture() const;
        sf::IntRect getGlyph( char c ) const;
        sf::IntRect getCropping( char c ) const;
        sf::Vector2i getSpacing() const;
        sf::Vector3i getKerning( char c ) const;
    
    private:
        sf::Texture tex;
        std::vector< sf::IntRect > glyphs;
        std::vector< sf::IntRect > cropping;
        std::vector< char > characters;
        sf::Vector2i spacing;
        std::vector< sf::Vector3i > kerning;
        std::size_t defaultCharIndex;
        
        std::size_t getCharIndex( char c ) const;
};

#endif // SPRITEFONT_HPP
