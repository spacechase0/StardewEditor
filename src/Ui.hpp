#ifndef UI_HPP
#define UI_HPP

#include <set>
#include <SFML/System/Clock.hpp>
#include <string>

class Editor;

namespace sf
{
    class Event;
    class RenderWindow;
}

class Ui
{
    public:
        Ui( Editor& theEditor );
        
        void update();
        void update( const sf::Event& event );
        void render( sf::RenderWindow& window );
        
        bool isMouseOutside() const;
    
    private:
        Editor& editor;
        sf::Clock delta;
        bool firstUpdate = true;

        std::set< std::string > maps;
        std::set< std::string > eventFiles;
        
        void initMapList();
        
        void mainMenu();
};

#endif // UI_HPP
