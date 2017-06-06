#ifndef XNB_DATA_HPP
#define XNB_DATA_HPP

#include <string>

#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class Data
    {
        public:
            //Data( const std::string& theType );
            virtual ~Data();
            
            //const std::string type;
            
            virtual std::string toString() const = 0;
            
            virtual std::vector< TypeReaderHeader > getTypeReaders() const;
            virtual std::string getType() const = 0;
            virtual TypeReaderHeader getTypeReader() const = 0;
    };
}

#endif // XNB_DATA_HPP
