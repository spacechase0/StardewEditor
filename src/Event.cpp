#include "Event.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <util/String.hpp>

namespace
{
    std::vector< Event::Precondition > parsePreconditions( const std::string& str )
    {
        std::vector< Event::Precondition > ret;
        
        auto tokens = util::tokenize( str, "/" );
        for ( const std::string& token : tokens )
        {
            Event::Precondition prec;
            prec.type = token[ 0 ];
            
            const auto& paramTypes = Event::PreconditionType::types[ prec.type ].paramTypes;
            if ( paramTypes.size() > 0 )
            {
                prec.params = util::tokenize( token.substr( 2 ), " " );
                if ( prec.params.size() != paramTypes.size() &&
                     std::find( paramTypes.begin(), paramTypes.end(), Event::ParamType::EnumMany ) == paramTypes.end() )
                {
                    util::log( "[WARN] Precondition parameters for \"$\" doesn't match its type. ($ vs $)\n", token, prec.params.size(), Event::PreconditionType::types[ prec.type ].paramTypes.size() );
                }
            }
            
            ret.push_back(  prec );
        }
        
        return ret;
    }
}

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
        std::size_t i = 0;
        for ( ; i < line.length(); ++i )
        {
            if ( !std::isspace( line[ i ] ) )
                break;
        }
        
        std::size_t colon = line.find( ':', i );
        if ( colon == std::string::npos )
            return Data();
        std::string key = line.substr( i, colon - i );
        
        std::string value;
        bool esc = false;
        for ( i = line.find( '"', colon + 1 ) + 1; i < line.length(); ++i )
        {
            char c = line[ i ];
            if ( c == '"' && !esc )
                break;
            else if ( c == '\\' && !esc )
                esc = true;
            else
            {
                value += c;
                esc = false;
            }
        }
        
        Data data;
        
        std::size_t slash = key.find( '/' );
        if ( slash == std::string::npos )
        {
            data.branchName = key;
        }
        else
        {
            data.id = util::fromString< int >( key.substr( 0, slash ) );
            data.preconditions = parsePreconditions( key.substr( slash + 1 ) );
        }
        
        return data;
    }
    
    std::string Data::toGameFormat() const
    {
        return "";
    }
}
