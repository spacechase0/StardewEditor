#ifndef MAP_HPP
#define MAP_HPP

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
};

#endif // MAP_HPP
