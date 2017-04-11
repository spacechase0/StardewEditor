#include "Editor.hpp"

#include <boost/filesystem.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

namespace fs = boost::filesystem;

const char* Editor::CONFIG_FILE = "cutscene-editor.cfg";

Editor::Editor( int argc, char* argv[] )
:   EXEC_PATH( fs::path( argv[ 0 ] ).parent_path().string() )
{
    auto configPath = fs::path( EXEC_PATH ) / CONFIG_FILE;
    if ( fs::exists( configPath ) )
        config.loadFromFile( configPath.string() );
    else config.saveToFile( CONFIG_FILE );
    
    initMapList();
}

Editor::~Editor()
{
    config.saveToFile( CONFIG_FILE );
}

void Editor::run()
{
    sf::RenderWindow window( sf::VideoMode( 640, 480 ), "Stardew Cutscene Editor" );
    window.setFramerateLimit( 60 );
    ImGui::SFML::Init( window );
    
    sf::Clock delta;
    bool isRunning = true;
    while ( isRunning )
    {
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            ImGui::SFML::ProcessEvent( event );
            if ( event.type == sf::Event::Closed )
            {
                isRunning = false;
            }
        }
        
        ImGui::SFML::Update( window, delta.restart() );
        update();
        
        window.clear( sf::Color::White );
        window.resetGLStates();
        render( window );
        ImGui::Render();
        window.display();
    }
    
    ImGui::SFML::Shutdown();
}

void Editor::changeCurrentMap( const std::string& map )
{
    if ( !currentMapTex.loadFromFile( ( fs::path( config.getMapImagesFolder() ) / ( map + ".png" ) ).string() ) )
    {
        std::cout << "Error: Couldn't load map image\n";
        return;
    }
    currentMap = map;
}

void Editor::update()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "File" ) )
        {
            if ( ImGui::BeginMenu( "Maps" ) )
            {
                bool refresh = false;
                ImGui::MenuItem( "(Refresh...)", nullptr, &refresh );
                if ( refresh )
                {
                    initMapList();
                }
                
                for ( const auto& map : maps )
                {
                    bool selected = currentMap == map;
                    ImGui::MenuItem( map.c_str(), nullptr, &selected, eventFiles.find( map ) != eventFiles.end() );
                    if ( selected && currentMap != map )
                    {
                        changeCurrentMap( map );
                    }
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void Editor::render( sf::RenderWindow& window )
{
    sf::View view( sf::FloatRect( 0, 0, window.getSize().x, window.getSize().y ) );
    window.setView( view );
    
    if ( currentMap != "" )
    {
        sf::Sprite spr( currentMapTex );
        window.draw( spr );
    }
}

void Editor::initMapList()
{
    maps.clear();
    for ( fs::directory_iterator it( config.getMapImagesFolder() );
          it != fs::directory_iterator(); ++it )
    {
        fs::path file = ( * it );
        if ( file.extension() == ".png" )
            maps.insert( file.stem().string() );
    }
    
    eventFiles.clear();
    for ( fs::directory_iterator it( fs::path( config.getUnpackedContentFolder() ) / "Data" / "Events" );
          it != fs::directory_iterator(); ++it )
    {
        fs::path file = ( * it );
        if ( file.extension() == ".yaml" && file.stem() == file.stem().stem() )
        {
            eventFiles.insert( file.stem().string() );
        }
    }
}
