#ifndef JPP_HPP
#define JPP_HPP

#include <functional>

/* require
  ../source/JPP.cpp
*/

namespace jpp{
  class JSON{
    

    // Parsing
    public: template < size_t BufferSize = 255 > void parse( const std::function< char() >& p_next );
    private: size_t parseString( char* p_buffer, const std::function< char() >& p_next );
    private: void parseNumber( const std::function< char() >& p_next );
  };
};

template < size_t BufferSize > void jpp::JSON::parse( const std::function< char() >& p_next ){
  char buffer [ BufferSize ];
  parseString( buffer, p_next );
}

#endif