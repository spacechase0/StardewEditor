#include "Editor.hpp"
/*
int main( int argc, char* argv[] )
{
    util::Logger::setName( "Main log", "log.txt" );
    
    Editor editor( argc, argv );
    editor.run();
}
*/

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "xnb/File.hpp"
#include "xnb/DictionaryType.hpp"
#include "xnb/TextureType.hpp"

int main()
{
    util::Logger::setName( "Main log", "log.txt" );
    
    xnb::File file;
    //if ( !file.loadFromFile( "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Stardew Valley\\Content\\Data\\eventConditions.xnb" ) )
    if ( !file.loadFromFile( "C:\\Users\\Chase\\Downloads\\alisci01-xnbdecompressor-26ea1b2f21fc\\alisci01-xnbdecompressor-26ea1b2f21fc\\bin\\Debug\\StardewContentDecompressed\\Characters\\Penny.xnb" ) )
    //if ( !file.loadFromFile( "C:\\Users\\Chase\\Downloads\\alisci01-xnbdecompressor-26ea1b2f21fc\\alisci01-xnbdecompressor-26ea1b2f21fc\\bin\\Debug\\StardewContentDecompressed\\Maps\\Farm.xnb" ) )
    //if ( !file.loadFromFile( "C:\\Users\\Chase\\Downloads\\alisci01-xnbdecompressor-26ea1b2f21fc\\alisci01-xnbdecompressor-26ea1b2f21fc\\bin\\Debug\\StardewContentDecompressed\\Data\\ObjectInformation.xnb" ) )
    //if ( !file.loadFromFile( "out.xnb" ) )
    {
        util::log( "Failed to load XNB file." );
        return 1;
    }
    util::log( "Loaded XNB file.\n" );
    
    if ( xnb::DictionaryData* dict = dynamic_cast< xnb::DictionaryData* >( file.data.get() ) )
    {
        util::log( "Dictionary: $ entries\n", dict->data.size() );
        for ( auto it = dict->data.begin(); it != dict->data.end(); ++it )
        {
            util::log( "\t$ = $\n", it->first->toString(), it->second->toString() );
        }
    }
    else if ( xnb::Texture2DData* texs = dynamic_cast< xnb::Texture2DData* >( file.data.get() ) )
    {
        util::log( "Texture2D: $x$, $ mips\n", texs->data[ 0 ].getSize().x, texs->data[ 0 ].getSize().y, texs->data.size() );
        
        sf::Texture tex;
        tex.loadFromImage( texs->data[ 0 ] );
        sf::Sprite spr( tex );
        
        sf::RenderWindow window( sf::VideoMode( texs->data[ 0 ].getSize().x, texs->data[ 0 ].getSize().y ), "XNB Texture Preview" );
        window.setFramerateLimit( 30 ); // Still overkill, but whatever
        while ( window.isOpen() )
        {
            sf::Event event;
            while ( window.pollEvent( event ) )
            {
                if ( event.type == sf::Event::Closed )
                    window.close();
                else if ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape )
                    window.close();
            }
            
            window.clear();
            window.draw( spr );
            window.display();
        }
    }
    else
    {
        util::log( "Data: $\n", file.data->toString() );
    }
    
    if ( !file.writeToFile( "out.xnb" ) )
        util::log( "Failed to write to file!\n" );
    else
    {
        xnb::File file2;
        if ( !file2.loadFromFile( "out.xnb" ) )
            util::log( "Failed to read file we just wrote!\n" );
    }
}
