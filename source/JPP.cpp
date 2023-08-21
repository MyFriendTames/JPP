#include "../include/JPP.hpp"

#include "JPP Exceptions.hpp"

jpp::JSON::Value jpp::JSON::parseString( char* p_buffer, const std::function< char() >& p_next ) const {
  jpp::JSON::Value result;
  size_t charactersRead = 0;
  char c;
  bool scape = false;
  uint8_t hex = 0;
  uint16_t hexValue;
  while ( ( c = p_next() ) != '"' || scape ){
    if ( c == '\0' ) throw jpp::UnexpectedEOF();
    else if ( hex ){
      if ( c >= '0' && c <= '9' ) hexValue = hexValue * 16 + ( c - '0' );
      else if ( c >= 'a' && c <= 'f' ) hexValue = hexValue * 16 + 10 + ( c - 'a' );
      else if ( c >= 'A' && c <= 'F' ) hexValue = hexValue * 16 + 10 + ( c - 'A');
      else throw jpp::MalformedHexValue();
      if ( !--hex ){
        // UTF-8 Encoding
        if ( hexValue <= 0x7F ) p_buffer[ charactersRead++ ] = static_cast< char >( hexValue );
        else if ( hexValue <= 0x7FF ){
          p_buffer[ charactersRead++ ] = static_cast< char >( 0b11000000 | ( hexValue >> 6 ) );
          p_buffer[ charactersRead++ ] = static_cast< char >( 0b10000000 | ( 0b00111111 & hexValue ) );
        }else{ // hexValue <= 0xFFFF
          p_buffer[ charactersRead++ ] = static_cast< char >( 0b11100000 | ( hexValue >> 12 ) );
          p_buffer[ charactersRead++ ] = static_cast< char >( 0b10000000 | ( 0b00111111 & ( hexValue >> 6 ) ) );
          p_buffer[ charactersRead++ ] = static_cast< char >( 0b10000000 | ( 0b00111111 & hexValue ) );
        }
        scape = false;
      }
    }else if ( c == '\\' ) scape = true;
    else if ( c == 'u' && scape ){
      hex = 4;
      hexValue = 0;
    }else{
      if ( scape ) c = (
        c == 'n' ? '\n' : (
          c == 'b' ? '\b' : (
            c == 'f' ? '\f' : (
              c == 't' ? '\t' : (
                c == 'r' ? '\r' : c
              )
            )
          )
        )
      );
      p_buffer[ charactersRead++ ] = c;
      scape = false;
    }
  }
  result.type = jpp::Type::String;
  result.strValue.str = new char [ charactersRead + 1 ];
  std::memcpy( result.strValue.str, p_buffer, charactersRead );
  result.strValue.str[ charactersRead ] = '\0';
  result.strValue.length = charactersRead;
  return result;
}

jpp::JSON::ParseResult jpp::JSON::parseNumber( char p_c, const std::function< char() >& p_next ) const {
  jpp::JSON::ParseResult result;
  bool negative = p_c == '-';
  size_t intPart = 0;
  float floatPart = 0;
  size_t decimalPoint = 0;
  do{
    if ( p_c >= '0' && p_c <= '9' ){
      if ( decimalPoint ){
        floatPart = floatPart * 10 + ( p_c - '0' );
        ++decimalPoint;
      }else intPart = intPart * 10 + ( p_c - '0' );
    }else if ( p_c == '.' && !decimalPoint ) decimalPoint = 1;
    else throw UnexpectedToken();
    p_c = p_next();
  }while ( p_c != '\0' && p_c != ',' && p_c != ' ' && p_c != '\n' && p_c != '\r' );
  // Finding the end
  do{
    if ( p_c == '\0' ) result.ending = jpp::JSON::ParseResult::Ending::EndOfFile;
    else if ( p_c == '}' ) result.ending = jpp::JSON::ParseResult::Ending::EndOfObject;
    else if ( p_c == ']' ) result.ending = jpp::JSON::ParseResult::Ending::EndOfList;
    else if ( p_c == ',' ) result.ending = jpp::JSON::ParseResult::Ending::EndOfValue;
    else if ( p_c != ' ' && p_c != '\n' && p_c != '\r' ) throw UnexpectedToken();
    else p_c = p_next();
  }while ( result.ending == ParseResult::Ending::Null );
  if ( decimalPoint ){
    result.value.type = jpp::Type::Float;
    result.value.floatValue = ( negative ? -1 : 1 ) * ( intPart + ( floatPart / std::pow( 10, decimalPoint - 1 ) ) );
  }else{
    result.value.type = jpp::Type::Integer;
    result.value.intValue = ( negative ? -1 : 1 ) * intPart;
  }
  return result;
}