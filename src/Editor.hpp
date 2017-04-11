#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <set>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "Config.hpp"
#include "Map.hpp"
#include "Ui.hpp"

namespace sf
{
    class RenderWindow;
}

class Editor
{
    public:
        static const char* CONFIG_FILE;
        
        Editor( int argc,  char* argv[] );
        ~Editor();
        
        void run();
        
        Config config;
        sf::RenderWindow window;
        Map map;
        Ui ui;
    
    private:
        const std::string EXEC_PATH;
};

#endif // EDITOR_HPP
