#ifndef XNB_FILE_HPP
#define XNB_FILE_HPP

#include <iostream>
#include <memory>
#include <string>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class File
    {
        public:
            bool loadFromFile( const std::string& path );
            bool loadFromStream( std::istream& in );
            
            bool writeToFile( const std::string& path );
            bool writeToStream( std::ostream& out );
            
            std::unique_ptr< xnb::Data > data;
            
            bool HiDef = false;
        
        private:
            bool readData( std::istream& in );
            bool writeData( std::ostream& out );
            
            friend class ITypeReader;
            std::vector< TypeReaderHeader > typeReaders;
    };
}

#endif // XNB_FILE_HPP
