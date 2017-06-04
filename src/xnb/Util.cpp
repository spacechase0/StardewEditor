#include "xnb/Util.hpp"

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
}
