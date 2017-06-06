#ifndef XNB_ITYPEREADER_HPP
#define XNB_ITYPEREADER_HPP

#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace xnb
{
    class Data;
    class File;
    
    struct TypeReaderHeader
    {
        std::string name;
        sf::Int32 version;
    };
    
    bool operator == ( const TypeReaderHeader& a, const TypeReaderHeader& b );
    
    class ITypeReader
    {
        public:
            virtual ~ITypeReader();
            
            virtual std::unique_ptr< Data > read( const File& file, std::istream& in, const std::string& fullDecl ) = 0;
            virtual void write( const File& file, std::ostream& out, const Data* data ) = 0;
            
            /// Can specify the type reader class or the resulting class (not including generic parameters)
            static ITypeReader* getTypeReader( std::string str );
            
            virtual bool resultIsValueType() const = 0;
            virtual std::string getReaderType() const = 0;
        
        protected:
            static std::string getTypeReaderNameAtIndex( const File& file, int index );
            static int getIndexOfTypeReader( const File& file, const std::string& reader );
    };
}

#endif // XNB_ITYPEREADER_HPP
