#ifndef UI_HPP
#define UI_HPP

#include <map>
#include <set>
#include <SFML/System/Clock.hpp>
#include <string>

#include "Event.hpp"

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
        std::map< int, Event::Data > events;
        std::map< std::string, Event::Data > eventBranches;
        
        void initMapList();
        void loadEventList( const std::string& map );
        
        void mainMenu();
};

#endif // UI_HPP
