#ifndef XNB_STRINGTYPE_HPP
#define XNB_STRINGTYPE_HPP

#include <memory>
#include <string>

#include "xnb/Data.hpp"
#include "xnb/ITypeReader.hpp"

namespace xnb
{
    class StringData : public Data
    {
        public:
            StringData( const std::string& str );
            std::string value;
            
            virtual std::string toString() const override;
    };
    
    class StringTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data >&& read( std::istream& in, const std::string& fullDecl ) override;
    };
    
    constexpr const char* STRING_TYPE = "System.String";
    constexpr const char* STRING_TYPEREADER = "Microsoft.Xna.Framework.Content.StringReader";
}

#endif // XNB_STRINGTYPE_HPP
