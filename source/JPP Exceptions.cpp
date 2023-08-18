#include "../include/JPP Exceptions.hpp"

const char* jpp::UnexpectedEOF::what() const throw(){
  return "Unexpected End Of File";
}

const char* jpp::UnexpectedToken::what() const throw(){
  return "Unexpected Token";
}

const char* jpp::MalformedHexValue::what() const throw(){
  return "Malformed Hexadecimal Value";
}