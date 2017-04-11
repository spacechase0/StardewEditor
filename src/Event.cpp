#include "Event.hpp"

#include <fstream>
#include <util/String.hpp>

namespace Event
{
    std::map< char, PreconditionType > PreconditionType::types = std::map< char, PreconditionType >();
    std::map< char, PreconditionType > PreconditionType::loadTypes( const std::string& path )
    {
        std::map< char, PreconditionType > ret;
        
        std::ifstream file( path );
        if ( !file )
            return ret;
        
        while ( true )
        {
            std::string str;
            std::getline( file, str );
            if ( !file )
                break;
            
            std::size_t eq1 = str.find( '=' );
            std::size_t eq2 = eq1 != std::string::npos ? str.find( '=', eq1 + 1 ) : std::string::npos;
            if ( eq1 != 1 || eq2 == std::string::npos )
                continue;
            
            PreconditionType prec;
            prec.id = str[ 0 ];
            prec.label = str.substr( eq2 + 1 );
            std::string params = str.substr( eq1 + 1, eq2 - eq1 - 1 );
            
            int paramCount = params[ 0 ] - '0';
            for ( int param = 0, i = 1; param< paramCount; ++param )
            {
                ParamType type = static_cast< ParamType >( params[ i ] );
                if ( type == ParamType::EnumOne || type == ParamType::EnumMany )
                {
                    std::size_t end = params.find( ';', i + 1 );
                    if ( end == std::string::npos )
                        continue;
                    std::string vals = params.substr( i + 1, end - i );
                    prec.enumValues = util::tokenize( vals, "," );
                }
                else ++i;
                prec.paramTypes.push_back( type );
            }
            
            ret.insert( std::make_pair( prec.id, prec ) );
        }
        
        return ret;
    }
    
    Data Data::fromGameFormat( const std::string& line )
    {
        Data data;
        data.branchName = line;
        return data;
    }
    
    std::string Data::toGameFormat() const
    {
        return "";
    }
}
