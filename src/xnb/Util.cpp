#include "xnb/Util.hpp"

#include<iostream>
namespace xnb
{
    sf::Int32 read7BitEncodedInt( std::istream& in )
    {
        int ret = 0;
        int bitsRead = 0;
        int value;
        
        do
        {
            value = in.get();
            ret |= ( value & 0x7F ) << bitsRead;
            bitsRead += 7;
        }
        while ( value & 0x80 );
        
        return ret;
    }
    
    void write7BitEncodedInt( std::ostream& out, sf::Int32 i )
    {
        do
        {
            char c = ( i & 0x7F );
            i = ( i >> 7 ) & ~0xFE0000000;
            if ( i != 0 )
                c |= 0x80;
            out.write( &c, 1 );
        }
        while ( i != 0 );
    }
}
