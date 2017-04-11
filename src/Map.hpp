#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>

class Editor;

namespace sf
{
    class Event;
    class RenderWindow;
}

class Map
{
    public:
        Map( Editor& editor );
        
        void update();
        void update( const sf::Event& event );
        void render( sf::RenderWindow& window );
        
        void changeCurrentMap( const std::string& map );
        std::string getCurrentMap() const;
    
    private:
        Editor& editor;
        
        std::string current;
        sf::Texture tex;
        sf::Sprite spr;
        
        bool dragging = false;
        sf::Vector2f dragFrom;
};

#endif // MAP_HPP
