#ifndef XNB_ITYPEREADER_HPP
#define XNB_ITYPEREADER_HPP

#include <istream>
#include <map>
#include <memory>
#include <string>

namespace xnb
{
    class Data;
    
    class ITypeReader
    {
        public:
            virtual ~ITypeReader();
            
            virtual std::unique_ptr< Data >&& read( std::istream& in, const std::string& fullDecl ) = 0;
            
            /// Can specify the type reader class or the resulting class (not including generic parameters)
            static ITypeReader* getTypeReader( const std::string& str );
        
        private:
    };
}

#endif // XNB_ITYPEREADER_HPP
