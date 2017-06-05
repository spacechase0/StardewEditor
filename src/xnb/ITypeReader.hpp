#ifndef XNB_ITYPEREADER_HPP
#define XNB_ITYPEREADER_HPP

#include <istream>
#include <map>
#include <memory>
#include <string>

namespace xnb
{
    class Data;
    class File;
    
    struct TypeReaderHeader
    {
        std::string name;
        sf::Int32 version;
    };
    
    class ITypeReader
    {
        public:
            virtual ~ITypeReader();
            
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) = 0;
            
            /// Can specify the type reader class or the resulting class (not including generic parameters)
            static ITypeReader* getTypeReader( std::string str );
            
            virtual bool resultIsValueType() const = 0;
        
        protected:
            static std::string getTypeReaderNameAtIndex( const File& file, int index );
    };
}

#endif // XNB_ITYPEREADER_HPP
