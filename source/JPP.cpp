#include "../include/JPP.hpp"

#include <iostream>

void jpp::JSON::parse( const std::function< char() >& next ){
  char c;
  while ( ( c = next() ) ){
    std::cout << c;
  }
}