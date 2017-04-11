#ifndef EVENT_HPP
#define EVENT_HPP

#include <map>
#include <string>
#include <vector>

namespace Event
{
    enum ParamType : char
    {
        Integer = 'i',
        Double = 'd',
        Bool = 'b',
        String = 's',
        EnumOne = 'e',
        EnumMany = 'E',
        
        Unknown = '?',
    };
    
    struct PreconditionType
    {
        char id;
        std::vector< ParamType > paramTypes;
        std::string label;
        
        /// We're assuming there is only one enum type per precondition, for now.
        std::vector< std::string > enumValues; 
        
        static std::map< char, PreconditionType > types;
        static std::map< char, PreconditionType > loadTypes( const std::string& path );
    };
    
    struct Precondition
    {
        std::vector< std::string > params;
    };
    
    struct Data
    {
        int id = 0;
        std::vector< Precondition > preconditions;
        
        std::string toGameFormat() const;
    };
}

#endif // EVENT_HPP
