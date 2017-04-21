#ifndef UI_HPP
#define UI_HPP

#include <map>
#include <set>
#include <SFML/System/Clock.hpp>
#include <string>

#include "Event.hpp"
#include "ui/SoundPlayer.hpp"

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
        
        std::vector< std::string > getSoundCueList() const;
        std::string getPathForSound( const std::string& cue ) const;
    
    private:
        Editor& editor;
        sf::Clock delta;
        bool firstUpdate = true;

        std::set< std::string > maps;
        std::set< std::string > eventFiles;
        std::map< int, Event::Data > events;
        std::map< std::string, Event::Data > eventBranches;
        Event::Data* active = nullptr;
        
        std::vector< std::string > precTypeLabels;
        std::string precTypeLabelsStr;
        std::map< char, std::string > enumValuesStr;
        
        SoundPlayer player;
        std::map< std::string, std::vector< std::string > > sounds;
        
        void initMapList();
        void loadEventList( const std::string& map );
        void reloadPreconditionTypes();
        void reloadSoundList();
        
        void mainMenu();
        void toolbar();
        void info();
        void preconditions();
        void actors();
        void commands();
};

#endif // UI_HPP
