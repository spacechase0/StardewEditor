#include "Editor.hpp"

#include <boost/filesystem.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace fs = boost::filesystem;

const char* Editor::CONFIG_FILE = "cutscene-editor.cfg";

Editor::Editor( int argc, char* argv[] )
:   map( * this ),
    ui( * this ),
    EXEC_PATH( fs::path( argv[ 0 ] ).parent_path().string() )
{
    auto configPath = fs::path( EXEC_PATH ) / CONFIG_FILE;
    if ( fs::exists( configPath ) )
        config.loadFromFile( configPath.string() );
    else config.saveToFile( CONFIG_FILE );
    
    util::Logger::setName( "Main log", "log.txt" );
}

Editor::~Editor()
{
    config.saveToFile( CONFIG_FILE );
}

void Editor::run()
{
    window.create( sf::VideoMode( 640, 480 ), "Stardew Cutscene Editor" );
    window.setFramerateLimit( 60 );
    ImGui::SFML::Init( window );
    
    bool isRunning = true;
    while ( isRunning )
    {
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed )
            {
                isRunning = false;
            }
            
            ui.update( event );
            map.update( event );
        }
        
        ui.update();
        map.update();
        
        window.clear( sf::Color::White );
        window.resetGLStates();
        map.render( window );
        ui.render( window );
        window.display();
    }
    
    ImGui::SFML::Shutdown();
}
