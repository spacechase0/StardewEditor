#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <set>
#include <SFML/Graphics/Texture.hpp>
#include <string>

#include "Config.hpp"

namespace sf
{
    class RenderWindow;
}

class Editor
{
    public:
        Editor( int argc,  char* argv[] );
        ~Editor();
        
        void run();
        
        void changeCurrentMap( const std::string& map );
        
        static const char* CONFIG_FILE;
        Config config;
    
    private:
        const std::string EXEC_PATH;
        
        std::set< std::string > maps;
        std::set< std::string > eventFiles;
        
        std::string currentMap;
        sf::Texture currentMapTex;
        
        void initMapList();
        
        void update();
        void render( sf::RenderWindow& window );
};

#endif // EDITOR_HPP
