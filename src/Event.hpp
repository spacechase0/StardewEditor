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
        char type;
        std::vector< std::string > params;
    };
    
    struct Data
    {
        // Either id or branch name.
        int id = -1;
        std::string branchName;
        
        std::vector< Precondition > preconditions;
        
        static Data fromGameFormat( const std::string& line );
        std::string toGameFormat() const;
    };
}

#endif // EVENT_HPP
