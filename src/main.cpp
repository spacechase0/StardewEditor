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
    if ( !file.loadFromFile( "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Stardew Valley\\Content\\Data\\eventConditions.xnb" ) )
    {
        util::log( "Failed to load XNB file." );
        return 0;
    }
}
