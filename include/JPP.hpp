#ifndef JPP
#define JPP

#include <functional>

/* require
  ../source/JPP.cpp
*/

namespace jpp{
  class JSON{
    public: void parse( const std::function< char() >& next );
  };
};

#endif