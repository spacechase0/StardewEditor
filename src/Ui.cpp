#include "Ui.hpp"

#include <boost/filesystem.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>
#include <util/File.hpp>
#include <util/String.hpp>

#include "Editor.hpp"
#include "ui/EventEditor.hpp"
#include "ui/SoundPlayer.hpp"

Ui::Ui( Editor& theEditor )
:   editor( theEditor )
{
}

void Ui::update()
{
    ImGui::SFML::Update( editor.window, delta.restart() );
    
    mainMenu();
    toolbar();
    other();
    for ( auto& module : modules )
    {
        module->update();
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
        
        modules.push_back( std::unique_ptr< UiModule >( eventEditor = new EventEditor( editor, ( * this ) ) ) );
        modules.push_back( std::unique_ptr< UiModule >( soundPlayer = new SoundPlayer( editor, ( * this ) ) ) );
        firstUpdate = false;
    }
    
    ImGui::Render();
}

bool Ui::isMouseOutside() const
{
    return !ImGui::IsMouseHoveringAnyWindow();
}

void Ui::showExport( const std::string& str, bool multi )
{
    exported = str;
    exportedMulti = multi;
}

void Ui::sendRefresh( Refresh::Type type )
{
    for ( auto& module : modules )
        module->refresh( type );
}

void Ui::initMapList()
{
    if ( !fs::exists( fs::path( editor.config.getDataFolder() ) / "maps" ) ||
         !fs::exists( fs::path( editor.config.getUnpackedContentFolder() ) / "Data" / "Events" ) )
        return;
    
    maps.clear();
    for ( fs::directory_iterator it( fs::path( editor.config.getDataFolder() ) / "maps" );
          it != fs::directory_iterator(); ++it )
    {
        fs::path file = ( * it );
        if ( file.extension() == ".png" )
            maps.insert( file.stem().string() );
    }
    
    sendRefresh( Refresh::MapList );
}

void Ui::mainMenu()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Maps" ) )
        {
            if ( ImGui::BeginMenu( "Reload" ) )
            {
                bool refresh = false;
                ImGui::MenuItem( "Map list", nullptr, &refresh );
                if ( refresh )
                {
                    initMapList();
                }
                
                ImGui::EndMenu();
            }
            ImGui::MenuItem( "", nullptr );
            
            for ( const auto& map : maps )
            {
                bool selected = editor.map.getCurrentMap() == map;
                ImGui::MenuItem( map.c_str(), nullptr, &selected/*, eventFiles.find( map ) != eventFiles.end()*/ );
                if ( selected && editor.map.getCurrentMap() != map )
                {
                    editor.map.changeCurrentMap( map );
                    sendRefresh( Refresh::Map );
                }
            }
            
            ImGui::EndMenu();
        }
        
        for ( auto& module : modules )
            module->menu();
        
        if ( ImGui::BeginMenu( "Other" ) )
        {
            bool wasShowingConfig = showingConfig;
            ImGui::MenuItem( "Show config window", nullptr, &showingConfig );
            if ( showingConfig != wasShowingConfig && !showingConfig )
            {
                editor.config.saveToFile( Editor::CONFIG_FILE );
            }
            
            bool selected = exported != "";
            ImGui::MenuItem( "Show export window", nullptr, &selected, selected );
            if ( !selected )
                exported = "";
            
            ImGui::EndMenu();
        }
        
        ImGui::EndMainMenuBar();
    }
}

void Ui::toolbar()
{
    int style = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    style |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
    style |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings;
    
    ImGui::SetNextWindowPos( ImVec2( -4, editor.window.getSize().y - 28 ) );
    ImGui::SetNextWindowSize( ImVec2( editor.window.getSize().x + 16, 28 ) );
    if ( ImGui::Begin( "", nullptr, style ) )
    {
        std::string tilePos = "";
        std::string pixelPos = "";
        if ( isMouseOutside() )
        {
            sf::Vector2f pixel = editor.map.pixelToWorld( sf::Mouse::getPosition( editor.window ) );
            sf::Vector2i tile( pixel.x / TILE_SIZE, pixel.y / TILE_SIZE );
            if ( pixel.x < 0 ) tile.x -= 1;
            if ( pixel.y < 0 ) tile.y -= 1;
            
            tilePos = util::format( "($, $)", tile.x, tile.y );
            pixelPos = util::format< int, int >( "($, $)", pixel.x, pixel.y );
        }
        ImGui::Text( ( "Tile: " + tilePos ).c_str() );
        ImGui::SameLine( 150 );
        ImGui::Text( ( "Pixel: " + pixelPos ).c_str() );
    }
    ImGui::End();
}

void Ui::other()
{
    if ( exported != "" )
    {
        if ( ImGui::Begin( "Exported" ) )
        {
            if ( exportedMulti )
                ImGui::InputTextMultiline( "", &exported[ 0 ], exported.length(), ImVec2( 0, 0 ), ImGuiInputTextFlags_ReadOnly );
            else
                ImGui::InputText( "", &exported[ 0 ], exported.length(), ImGuiInputTextFlags_ReadOnly );
        }
        ImGui::End();
    }
    
    if ( showingConfig )
    {
        if ( ImGui::Begin( "Configuration" ) )
        {
            ImGui::InputText( "Unpacked content", &editor.config.unpackedContent[ 0 ], 511 );
            //ImGui::InputText( "Data path", &editor.config.dataFolder[ 0 ], 511 );
            ImGui::InputText( "Extracted sounds", &editor.config.extractedSounds[ 0 ], 511 );
        }
        ImGui::End();
    }
}
