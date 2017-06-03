#ifndef XNB_FILE_HPP
#define XNB_FILE_HPP

#include <iostream>
#include <string>

namespace xnb
{
    class File
    {
        public:
            bool loadFromFile( const std::string& path );
            bool loadFromStream( std::istream& in );
        
        private:
            bool readData( std::istream& in );
    };
}

#endif // XNB_FILE_HPP
