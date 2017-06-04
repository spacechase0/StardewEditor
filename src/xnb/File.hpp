#ifndef XNB_FILE_HPP
#define XNB_FILE_HPP

#include <iostream>
#include <memory>
#include <string>

#include "xnb/Data.hpp"

namespace xnb
{
    class File
    {
        public:
            bool loadFromFile( const std::string& path );
            bool loadFromStream( std::istream& in );
            
            std::unique_ptr< xnb::Data > data;
        
        private:
            bool readData( std::istream& in );
    };
}

#endif // XNB_FILE_HPP
