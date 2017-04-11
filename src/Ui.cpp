#include "Ui.hpp"

#include <boost/filesystem.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics/Sprite.hpp>
#include <util/String.hpp>

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
    if ( active )
    {
        info();
        preconditions();
        commands();
    }
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
    active = nullptr;
    
    std::ifstream file( ( fs::path( editor.config.getUnpackedContentFolder() ) / "data" / "Events" / ( map + ".yaml" ) ).string() );
    if ( !file )
        return;
    
    bool foundContent = false;
    while ( true )
    {
        std::string line;
        std::getline( file, line );
        if ( !file )
            break;
        
        if ( !foundContent )
        {
            if ( line.length() < 8 || line.substr( 0, 8 ) != "content:" )
                continue;
            foundContent = true;
        }
        else if ( line.length() >= 4 )
        {
            Event::Data event = Event::Data::fromGameFormat( line );
            if ( event.id == -1 && event.branchName == "" )
                continue;
            
            if ( event.id != -1 )
            {
                events.insert( std::make_pair( event.id, event ) );
            }
            else
            {
                eventBranches.insert( std::make_pair( event.branchName, event ) );
            }
        }
    }
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
                
                for ( auto& event : events )
                {
                    bool selected = active == &event.second;
                    ImGui::MenuItem( util::toString( event.first ).c_str(), nullptr, &selected, true );
                    if ( selected )
                    {
                        active = &event.second;
                    }
                }
                
                for ( auto& event : eventBranches )
                {
                    bool selected = active == &event.second;
                    ImGui::MenuItem( event.first.c_str(), nullptr, &selected, true );
                    if ( selected )
                    {
                        active = &event.second;
                    }
                }
                
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if ( ImGui::BeginMenu( "Reload" ) )
        {
            bool refresh = false;
            ImGui::MenuItem( "Precondition types", nullptr, &refresh );
            if ( refresh )
            {
                Event::PreconditionType::types = Event::PreconditionType::loadTypes( ( fs::path( editor.config.getDataFolder() ) / "preconditions.txt" ).string() );
            }
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void Ui::info()
{
    ImGui::SetNextWindowPos( ImVec2( 25, 25 ), ImGuiSetCond_Appearing );
    ImGui::SetNextWindowSize( ImVec2( 250, 100 ), ImGuiSetCond_Appearing );
    if ( ImGui::Begin( "Info" ) )
    {
        if ( active->id != -1 )
            ImGui::InputInt( "Event ID", &active->id );
        else
            ImGui::InputText( "Event Name", &active->branchName[ 0 ], 31 );
        
        ImGui::InputText( "Music", &active->music[ 0 ], 31 );
        ImGui::InputInt2( "Viewport", &active->viewport.x );
    }
    ImGui::End();
}

void Ui::preconditions()
{
    if ( active->id == -1 )
        return;
    
    ImGui::SetNextWindowPos( ImVec2( 25, 150 ), ImGuiSetCond_Appearing );
    ImGui::SetNextWindowSize( ImVec2( 250, 200 ), ImGuiSetCond_Appearing );
    if ( ImGui::Begin( "Preconditions" ) )
    {
    }
    ImGui::End();
}

void Ui::commands()
{
    ImGui::SetNextWindowPos( ImVec2( editor.window.getSize().x - 25 - 250, 25 ), ImGuiSetCond_Appearing );
    ImGui::SetNextWindowSize( ImVec2( 250, 500 ), ImGuiSetCond_Appearing );
    if ( ImGui::Begin( "Commands" ) )
    {
    }
    ImGui::End();
}
