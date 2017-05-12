#ifndef UI_HPP
#define UI_HPP

#include <map>
#include <set>
#include <SFML/System/Clock.hpp>
#include <string>

#include "Event.hpp"
#include "ui/UiModule.hpp"

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
        
        std::vector< std::unique_ptr< UiModule > > modules;
        
        bool showingConfig = false;

        std::set< std::string > maps;
        std::set< std::string > eventFiles;
        std::map< int, Event::Data > events;
        std::map< std::string, Event::Data > eventBranches;
        Event::Data* active = nullptr;
        Event::Data dummy;
        
        std::string exported;
        bool exportedMulti = false;
        
        std::vector< std::string > precTypeLabels;
        std::string precTypeLabelsStr;
        std::map< char, std::string > enumValuesStr;
        
        void initMapList();
        void loadEventList( const std::string& map );
        void reloadPreconditionTypes();
        
        void mainMenu();
        void toolbar();
        void other();
        void info();
        void preconditions();
        void actors();
        void commands();
};

#endif // UI_HPP
