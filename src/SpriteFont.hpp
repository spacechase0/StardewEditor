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
    
    private:
        sf::Texture tex;
        std::vector< sf::IntRect > glyphs;
        std::vector< sf::IntRect > cropping;
        std::vector< char > characters;
        sf::Vector2i spacing;
        std::vector< sf::Vector3i > kerning;
        char defaultChar;
};

#endif // SPRITEFONT_HPP
