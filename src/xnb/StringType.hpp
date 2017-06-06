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
            
            virtual std::string getType() const override;
            virtual TypeReaderHeader getTypeReader() const override;
    };
    
    class StringTypeReader : public ITypeReader
    {
        public:
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) override;
            virtual void write( const File& file, std::ostream& out, const Data* data ) override;
            
            virtual bool resultIsValueType() const override;
            virtual std::string getReaderType() const override;
    };
    
    constexpr const char* STRING_TYPE = "System.String";
    constexpr const char* STRING_TYPEREADER = "Microsoft.Xna.Framework.Content.StringReader";
}

#endif // XNB_STRINGTYPE_HPP
