#ifndef ___JPP_JSON_INTERNAL_STRUCTURES___
#define ___JPP_JSON_INTERNAL_STRUCTURES___

/* require
  ../source/JPP InternalStructures.cpp
*/

struct jpp::JSON::Value{
  jpp::Type type;
  union{
    struct{
      size_t length;
      char* str;
    } strValue;
    size_t intValue;
    float floatValue;
    bool boolValue;
  };
};

class jpp::JSON::ParseResult{
  public: enum class Ending{
    Null = 0,
    EndOfFile,
    EndOfList,
    EndOfObject,
    EndOfValue
  };
  public: Ending ending;
  public: jpp::JSON::Value value;
  public: ParseResult();
};

#endif