#ifndef JPP_HPP
#define JPP_HPP

#include <functional>
#include <cstring>

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
    private: struct Value{
      jpp::Type type;
      union{
        struct{
          size_t length;
          char* str;
        } strValue;
        size_t intValue;
        float floatValue;
        bool boolValue;
        bool nullValue;
      };
    };

    // Parsing
    public: template < size_t BufferSize = 255 > void parse( const std::function< char() >& p_next ) const;
    private: jpp::JSON::Value parseString( char* p_buffer, const std::function< char() >& p_next ) const;
    private: jpp::JSON::Value parseNumber( char p_c, const std::function< char() >& p_next ) const;
  };
};

#include <iostream>

template < size_t BufferSize > void jpp::JSON::parse( const std::function< char() >& p_next ) const {
  char buffer [ BufferSize ];
  jpp::JSON::Value result = parseString( buffer, p_next );
  std::cout << "String: \"" << result.strValue.str << "\"" << std::endl;
  delete [] result.strValue.str;
}

#endif