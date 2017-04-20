#include "Ui.hpp"

#include <boost/filesystem.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>
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
    toolbar();
    if ( active )
    {
        info();
        preconditions();
        actors();
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
        reloadPreconditionTypes();
        firstUpdate = false;
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

void Ui::reloadPreconditionTypes()
{
    Event::PreconditionType::types = Event::PreconditionType::loadTypes( ( fs::path( editor.config.getDataFolder() ) / "preconditions.txt" ).string() );
    precTypeLabels.clear();
    for ( const auto& type : Event::PreconditionType::types )
    {
        precTypeLabels.push_back( type.second.label );
        
        if ( type.second.enumValues.size() > 0 )
        {
            std::string str = "";
            for ( const std::string& val : type.second.enumValues )
            {
                str += val + '\0';
            }
            str += '\0';
            
            enumValuesStr[ type.second.id ] = str;
        }
    }
    
    precTypeLabelsStr = "";
    for ( const std::string& label : precTypeLabels )
    {
        precTypeLabelsStr += label + '\0';
    }
    precTypeLabelsStr += '\0';
}

void Ui::mainMenu()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Maps" ) )
        {
            bool refresh = false;
            ImGui::MenuItem( "(Refresh...)", nullptr, &refresh );
            if ( refresh )
            {
                initMapList();
            }
            ImGui::MenuItem( "", nullptr );
            
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
            if ( ImGui::BeginMenu( "New" ) )
            {
                bool selected = false;
                ImGui::MenuItem( "w/ ID", nullptr, &selected );
                if ( selected )
                {
                    /// TODO: Implement
                    //events.insert();
                }
                
                selected = false;
                ImGui::MenuItem( "Named", nullptr, &selected );
                if ( selected )
                {
                    /// TODO: Implement
                    //events.insert();
                }
                
                ImGui::EndMenu();
            }
            
            bool refresh = false;
            ImGui::MenuItem( "(Refresh...)", nullptr, &refresh );
            if ( refresh )
            {
                loadEventList( editor.map.getCurrentMap() );
            }
            ImGui::MenuItem( "", nullptr );
            
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
        if ( ImGui::BeginMenu( "Reload" ) )
        {
            bool refresh = false;
            ImGui::MenuItem( "Precondition types", nullptr, &refresh );
            if ( refresh )
            {
                reloadPreconditionTypes();
            }
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
    
    ImGui::SetNextWindowPos( ImVec2( -8, editor.window.getSize().y - 28 ) );
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

void Ui::info()
{
    ImGui::SetNextWindowPos( ImVec2( 25, 25 ), ImGuiSetCond_Appearing );
    ImGui::SetNextWindowSize( ImVec2( 250, 100 ), ImGuiSetCond_Appearing );
    if ( ImGui::Begin( "Event Info" ) )
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
    if ( ImGui::Begin( "Event Preconditions" ) )
    {
        int precNum = 0;
        for ( auto precIt = active->preconditions.begin(); precIt != active->preconditions.end(); ++precIt, ++precNum )
        {
            Event::Precondition& prec = ( * precIt );
            const Event::PreconditionType& type = Event::PreconditionType::types[ prec.type ];
            int selPrec = std::find( precTypeLabels.begin(), precTypeLabels.end(), type.label ) - precTypeLabels.begin();
            int oldSelPrec = selPrec;
            ImGui::Combo( util::format( "Type##prec$", precNum ).c_str(), &selPrec, precTypeLabelsStr.c_str() );
            if ( selPrec != oldSelPrec )
            {
                for ( const auto& checkType : Event::PreconditionType::types )
                {
                    if ( checkType.second.label == precTypeLabels[ selPrec ] )
                    {
                        prec = Event::Precondition::init( checkType.second );
                        break;
                    }
                }
                continue;
            }
            
            std::size_t iVal = 0;
            for ( std::size_t i = 0; i < type.paramTypes.size(); ++i, ++iVal )
            {
                switch ( type.paramTypes[ i ] )
                {
                    case Event::ParamType::Integer:
                        {
                            int x = util::fromString< int >( prec.params[ iVal ] );
                            int oldX = x;
                            ImGui::InputInt( util::format( "$##prec$param$", type.paramLabels[ i ], precNum, i ).c_str(), &x );
                            if ( x != oldX )
                                prec.params[ iVal ] = util::toString( x );
                        }
                        break;
                    
                    case Event::ParamType::Double:
                        {
                            float x = util::fromString< float >( prec.params[ iVal ] );
                            float oldX = x;
                            ImGui::InputFloat( util::format( "$##prec$param$", type.paramLabels[ i ], precNum, i ).c_str(), &x );
                            if ( x != oldX )
                                prec.params[ iVal ] = util::toString( x );
                        }
                        break;
                    
                    case Event::ParamType::Bool:
                        {
                            bool x = prec.params[ iVal ] == "true";
                            bool oldX = x;
                            ImGui::Checkbox( util::format( "$##prec$param$", type.paramLabels[ i ], precNum, i ).c_str(), &x );
                            if ( x != oldX )
                                prec.params[ iVal ] = x ? "true" : "false";
                        }
                        break;
                    
                    case Event::ParamType::String:
                    case Event::ParamType::Unknown:
                        {
                            prec.params[ iVal ].resize( 32, '\0' );
                            ImGui::InputText( util::format( "$##prec$param$", type.paramLabels[ i ], precNum, i ).c_str(), &prec.params[ iVal ][ 0 ], 31 );
                        }
                        break;
                    
                    case Event::ParamType::EnumOne:
                        {
                            int selEnum = std::find( type.enumValues.begin(), type.enumValues.end(), prec.params[ iVal ] ) - type.enumValues.begin();
                            int oldSelEnum = selEnum;
                            ImGui::Combo( util::format( "##prec$param$", precNum, i ).c_str(), &selEnum, &enumValuesStr[ type.id ][ 0 ] );
                            if ( selEnum != oldSelEnum )
                                prec.params[ iVal ] = type.enumValues[ selEnum ];
                        }
                        break;
                    
                    case Event::ParamType::EnumMany:
                        {
                            for ( std::size_t e = 0; e < type.enumValues.size(); ++e )
                            {
                                const std::string& val = type.enumValues[ e ];
                                auto valIt = std::find( prec.params.begin(), prec.params.end(), val );
                                bool sel = valIt != prec.params.end();
                                bool oldSel = sel;
                                ImGui::Checkbox( util::format( "$##prec$param$val$", val, precNum, i, e ).c_str(), &sel );
                                if ( sel != oldSel )
                                {
                                    if ( sel )
                                        prec.params.push_back( val );
                                    else
                                        prec.params.erase( valIt );
                                }
                            }
                        }
                        break;
                    
                    case Event::ParamType::Position:
                        {
                            sf::Vector2i x( util::fromString< int >( prec.params[ iVal ] ), util::fromString< int >( prec.params[ iVal + 1 ] ) );
                            sf::Vector2i oldX;
                            ImGui::InputInt2( util::format( "$##prec$param$", type.paramLabels[ i ], precNum, i ).c_str(), &x.x );
                            if ( x != oldX )
                            {
                                prec.params[ i ] = util::toString( x.x );
                                prec.params[ i + 1 ] = util::toString( x.y );
                            }
                            ++iVal;
                        }
                        break;
                }
            }
            if ( ImGui::Button( util::format( "Delete precondition##prec$", precNum ).c_str() ) )
            {
                active->preconditions.erase( precIt );
                break;
            }
            ImGui::Separator();
        }
        if ( ImGui::Button( "New precondition" ) )
        {
            active->preconditions.push_back( Event::Precondition::init( Event::PreconditionType::types.begin()->second ) );
        }
    }
    ImGui::End();
}

void Ui::actors()
{
    ImGui::SetNextWindowPos( ImVec2( 25, 375 ), ImGuiSetCond_Appearing );
    ImGui::SetNextWindowSize( ImVec2( 250, 200 ), ImGuiSetCond_Appearing );
    if ( ImGui::Begin( "Event Actors" ) )
    {
        int actorNum = 0;
        for ( auto actorIt = active->actors.begin(); actorIt != active->actors.end(); ++actorIt, ++actorNum )
        {
            Event::Actor& actor = ( * actorIt );
            actor.name.resize( 32, '\0' );
            ImGui::InputText( util::format( "Name##actor$", actorNum ).c_str(), &actor.name[ 0 ], 31 );
            ImGui::InputInt2( util::format( "Position##actor$", actorNum ).c_str(), &actor.pos.x );
            ImGui::InputInt( util::format( "Facing##actor$", actorNum ).c_str(), &actor.facing );
            actor.facing = actor.facing % 4;
            
            if ( ImGui::Button( util::format( "Delete actor##actor$", actorNum ).c_str() ) )
            {
                active->actors.erase( actorIt );
                break;
            }
            
            ImGui::Separator();
        }
        
        if ( ImGui::Button( "New actor" ) )
        {
            active->actors.push_back( Event::Actor() );
        }
    }
    ImGui::End();
}

void Ui::commands()
{
    ImGui::SetNextWindowPos( ImVec2( editor.window.getSize().x - 25 - 250, 25 ), ImGuiSetCond_Appearing );
    ImGui::SetNextWindowSize( ImVec2( 250, 500 ), ImGuiSetCond_Appearing );
    if ( ImGui::Begin( "Event Commands" ) )
    {
        int cmdNum = 0;
        for ( auto cmdIt = active->commands.begin(); cmdIt != active->commands.end(); ++cmdIt, ++cmdNum )
        {
            Event::Command& command = ( * cmdIt );
            command.cmd.resize( 512, '\0' );
            ImGui::InputText( util::format( "Command##cmd$", cmdNum ).c_str(), &command.cmd[ 0 ], 511 );
            
            if ( ImGui::Button( util::format( "Delete command##cmd$", cmdNum ).c_str() ) )
            {
                active->commands.erase( cmdIt );
                break;
            }
            
            ImGui::Separator();
        }
        
        if ( ImGui::Button( "New command" ) )
        {
            active->commands.push_back( Event::Command() );
        } 
    }
    ImGui::End();
}
