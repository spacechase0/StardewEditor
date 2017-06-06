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
    
    std::string StringData::getType() const
    {
        return STRING_TYPE;
    }
    
    TypeReaderHeader StringData::getTypeReader() const
    {
        return TypeReaderHeader{ STRING_TYPEREADER, 0 };
    }
    
    std::unique_ptr< Data > StringTypeReader::read( const File& file, std::istream& in, const std::string& fullDecl )
    {
        int bytes = read7BitEncodedInt( in );
        std::string str( bytes, '\0' );
        in.read( &str[ 0 ], bytes );
        
        return std::move( std::unique_ptr< Data >( new StringData( str ) ) );
    }
    
    void StringTypeReader::write( const File& file, std::ostream& out, const Data* data )
    {
        const StringData* str = dynamic_cast< const StringData* >( data );
        if ( str == nullptr )
            throw std::invalid_argument( "Bad data type passed to write" );
        
        write7BitEncodedInt( out, str->value.size() );
        out.write( str->value.c_str(), str->value.size() );
    }
    
    bool StringTypeReader::resultIsValueType() const
    {
        return false;
    }
    
    std::string StringTypeReader::getReaderType() const
    {
        return STRING_TYPEREADER;
    }
}
