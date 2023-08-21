#ifndef ___JPP_JSON_HPP___
#define ___JPP_JSON_HPP___

#include <functional>
#include <cstring>
#include <cmath>

/* require
  ../source/JPP.cpp
*/

namespace jpp{
  enum class Type{
    String = 0,
    Integer,
    Float,
    Boolean,
    Null
  };

  class JSON{
    private: struct Value;
    private: struct ParseResult;

    // Parsing
    public: template < size_t BufferSize = 255 > void parse( const std::function< char() >& p_next ) const;
    private: jpp::JSON::Value parseString( char* p_buffer, const std::function< char() >& p_next ) const;
    private: jpp::JSON::ParseResult parseNumber( char p_c, const std::function< char() >& p_next ) const;
  };
};

#include "JPP InternalStructures.hpp"

#include <iostream>

template < size_t BufferSize > void jpp::JSON::parse( const std::function< char() >& p_next ) const {
  char buffer [ BufferSize ];
  
  //jpp::JSON::Value result = parseString( buffer, p_next );
  //std::cout << "String: \"" << result.strValue.str << "\"" << std::endl;
  //delete [] result.strValue.str;

  jpp::JSON::ParseResult res;
  do{
    char c;
    do{
      c = p_next();
    }while ( c == ' ' || c == '\n' || c == '\r' );
    res = parseNumber( c, p_next );
    if ( res.value.type == jpp::Type::Float ) std::cout << "Float: " << res.value.floatValue << std::endl;
    else if ( res.value.type == jpp::Type::Integer ) std::cout << "Integer: " << res.value.intValue << std::endl;
  }while ( res.ending == jpp::JSON::ParseResult::Ending::EndOfValue );
}

#endif