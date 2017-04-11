#include "Ui.hpp"

#include <boost/filesystem.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics/Sprite.hpp>

#include "Editor.hpp"
#include "Event.hpp"

Ui::Ui( Editor& theEditor )
:   editor( theEditor )
{
}

void Ui::update()
{
    ImGui::SFML::Update( editor.window, delta.restart() );
    
    mainMenu();
}

void Ui::update( const sf::Event& event )
{
    ImGui::SFML::ProcessEvent( event );
}

void Ui::render( sf::RenderWindow& window )
{
    if ( firstUpdate )
    {
        initMapList();
        firstUpdate = false;
        
        Event::PreconditionType::types = Event::PreconditionType::loadTypes( ( fs::path( editor.config.getDataFolder() ) / "preconditions.txt" ).string() );
    }
    
    ImGui::Render();
}

bool Ui::isMouseOutside() const
{
    return !ImGui::IsMouseHoveringAnyWindow();
}

void Ui::initMapList()
{
    maps.clear();
    for ( fs::directory_iterator it( fs::path( editor.config.getDataFolder() ) / "maps" );
          it != fs::directory_iterator(); ++it )
    {
        fs::path file = ( * it );
        if ( file.extension() == ".png" )
            maps.insert( file.stem().string() );
    }
    
    eventFiles.clear();
    for ( fs::directory_iterator it( fs::path( editor.config.getUnpackedContentFolder() ) / "Data" / "Events" );
          it != fs::directory_iterator(); ++it )
    {
        fs::path file = ( * it );
        if ( file.extension() == ".yaml" && file.stem() == file.stem().stem() )
        {
            eventFiles.insert( file.stem().string() );
        }
    }
}

void Ui::loadEventList( const std::string& map )
{
    events.clear();
    eventBranches.clear();
}

void Ui::mainMenu()
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
                    bool selected = editor.map.getCurrentMap() == map;
                    ImGui::MenuItem( map.c_str(), nullptr, &selected, eventFiles.find( map ) != eventFiles.end() );
                    if ( selected && editor.map.getCurrentMap() != map )
                    {
                        editor.map.changeCurrentMap( map );
                        loadEventList( map );
                    }
                }
                
                ImGui::EndMenu();
            }
            if ( ImGui::BeginMenu( "Events" ) )
            {
                bool refresh = false;
                ImGui::MenuItem( "(Refresh...)", nullptr, &refresh );
                if ( refresh )
                {
                        loadEventList( editor.map.getCurrentMap() );
                }
                
                // ...
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}
