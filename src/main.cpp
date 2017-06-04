#include "Editor.hpp"
/*
int main( int argc, char* argv[] )
{
    util::Logger::setName( "Main log", "log.txt" );
    
    Editor editor( argc, argv );
    editor.run();
}
*/

#include "xnb/File.hpp"

int main()
{
    util::Logger::setName( "Main log", "log.txt" );
    
    xnb::File file;
    //if ( !file.loadFromFile( "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Stardew Valley\\Content\\Data\\eventConditions.xnb" ) )
    //if ( !file.loadFromFile( "C:\\Users\\Chase\\Downloads\\alisci01-xnbdecompressor-26ea1b2f21fc\\alisci01-xnbdecompressor-26ea1b2f21fc\\bin\\Debug\\StardewContentDecompressed\\Data\\Achievements.xnb" ) )
    if ( !file.loadFromFile( "C:\\Users\\Chase\\Downloads\\alisci01-xnbdecompressor-26ea1b2f21fc\\alisci01-xnbdecompressor-26ea1b2f21fc\\bin\\Debug\\StardewContentDecompressed\\Data\\eventConditions.xnb" ) )
    {
        util::log( "Failed to load XNB file." );
        return 1;
    }
    util::log( "Loaded XNB file.\n" );
    
    util::log( "Data: $\n", file.data.get() );
    util::log( "Data: $\n", file.data->toString() );
}
