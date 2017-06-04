#ifndef XNB_UTIL_HPP
#define XNB_UTIL_HPP

#include <istream>
#include <SFML/Config.hpp>

namespace xnb
{
    sf::Int32 read7BitEncodedInt( std::istream& in );
}

#endif // XNB_UTIL_HPP
