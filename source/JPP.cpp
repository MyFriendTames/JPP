#include "../include/JPP.hpp"

#include <iostream>
#include "JPP Exceptions.hpp"

size_t jpp::JSON::parseString( char* p_buffer, const std::function< char() >& p_next ){
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
  p_buffer[ charactersRead ] = '\0';
  std::cout << "String: \"" << p_buffer << "\"" << " # " << charactersRead << std::endl;
  return charactersRead;
}