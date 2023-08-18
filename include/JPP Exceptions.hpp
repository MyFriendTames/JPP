#ifndef JPP_EXCEPTIONS_HPP
#define JPP_EXCEPTIONS_HPP

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