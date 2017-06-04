#include "xnb/StringType.hpp"

#include "xnb/Util.hpp"

namespace xnb
{
    StringData::StringData( const std::string& str )
    :   value( str )
    {
    }
    
    std::string StringData::toString() const
    {
        return '"' + value + '"';
    }
    
    std::unique_ptr< Data >&& StringTypeReader::read( std::istream& in, const std::string& fullDecl )
    {
        int bytes = read7BitEncodedInt( in );
        std::string str( bytes, '\0' );
        in.read( &str[ 0 ], bytes );
        
        return std::move( std::unique_ptr< Data >( new StringData( str ) ) );
    }
}
