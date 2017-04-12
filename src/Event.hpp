#ifndef EVENT_HPP
#define EVENT_HPP

#include <map>
#include <SFML/System/Vector2.hpp>
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
        
        static Precondition init( PreconditionType type );
    };
    
    struct Actor
    {
        std::string name;
        sf::Vector2i pos;
        int facing;
    };
    
    struct Command
    {
        std::string cmd;
        std::vector< std::string > params;
    };
    
    struct Data
    {
        Data();
        
        // Either id or branch name.
        int id = -1;
        std::string branchName;
        
        std::vector< Precondition > preconditions;
        
        std::string music;
        sf::Vector2i viewport;
        std::vector< Actor > actors;
        
        std::vector< Command > commands;
        
        static Data fromGameFormat( const std::string& line );
        std::string toGameFormat() const;
    };
}

#endif // EVENT_HPP
