#ifndef ___JPP_JSON_EXCEPTIONS_HPP___
#define ___JPP_JSON_EXCEPTIONS_HPP___

#include <exception>

/* require
  ../source/JPP Exceptions.cpp
*/

namespace jpp{
  class UnexpectedEOF : public std::exception{
    virtual const char* what() const throw();
  };

  class UnexpectedToken : public std::exception{
    virtual const char* what() const throw();
  };

  class MalformedHexValue : public std::exception{
    virtual const char* what() const throw();
  };
};

#endif