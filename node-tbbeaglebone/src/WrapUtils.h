#ifndef __INCLUDE_NODE_UTILS_H
#define __INCLUDE_NODE_UTILS_H

#include <stdexcept>
#include <v8.h>

class WrapUtils {
  public:
    static v8::Handle<v8::Value> makeErrorValue(const char* fmt, ...);
    static v8::Handle<v8::Value> makeErrorValue(const std::exception& e);

  private:
    WrapUtils(); // static only
};

#define RETURN_EXCEPTIONS_AS_NODE_ERRORS(block) \
  try { \
    block \
  } catch (const exception& e) { \
    return WrapUtils::makeErrorValue(e); \
  }

#define CALL_WRAPPED_METHOD(T, method, methodArgs...) \
  { \
    T* instance = ObjectWrap::Unwrap<T>(args.This()); \
    RETURN_EXCEPTIONS_AS_NODE_ERRORS({ \
      instance->method(methodArgs); \
    }) \
  }

#define CALL_WRAPPED_METHOD_WITH_RETURN(T, returnVar, method, methodArgs...) \
  { \
    T* instance = ObjectWrap::Unwrap<T>(args.This()); \
    RETURN_EXCEPTIONS_AS_NODE_ERRORS({ \
      returnVar = instance->method(methodArgs); \
    }) \
  }

#endif
