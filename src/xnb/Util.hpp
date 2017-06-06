#ifndef XNB_UTIL_HPP
#define XNB_UTIL_HPP

#include <istream>
#include <ostream>
#include <SFML/Config.hpp>

namespace xnb
{
    sf::Int32 read7BitEncodedInt( std::istream& in );
    void write7BitEncodedInt( std::ostream& out, sf::Int32 i );
}

#endif // XNB_UTIL_HPP
