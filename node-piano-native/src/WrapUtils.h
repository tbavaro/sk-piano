#ifndef __INCLUDE_NODE_UTILS_H
#define __INCLUDE_NODE_UTILS_H

#include "Log.h"
#include <stdexcept>
#include <v8.h>

class WrapUtils {
  public:
    static v8::Handle<v8::Value> makeErrorValue(const char* fmt, ...);
    static v8::Handle<v8::Value> makeErrorValue(const std::exception& e);

    static v8::Persistent<v8::FunctionTemplate> wrapConstructor(
        const char* className, v8::InvocationCallback newFunc);

  private:
    WrapUtils(); // static only
};

#define RETURN_EXCEPTIONS_AS_NODE_ERRORS(block) \
  try { \
    RUN_LOGGING_EXCEPTION(block) \
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

#define SET_MEMBER_WITH_ATTRIBS(target, name, value, attribs) \
  target->Set(v8::String::NewSymbol(name), value, attribs)

#define SET_MEMBER(target, name, value) \
  SET_MEMBER_WITH_ATTRIBS(target, name, value, (PropertyAttribute)None)

#define SET_INT_MEMBER_WITH_ATTRIBS(target, name, value, attribs) \
  SET_MEMBER_WITH_ATTRIBS(target, name, v8::Integer::New(value), attribs)

#define SET_INT_MEMBER(target, name, value) \
  SET_INT_MEMBER_WITH_ATTRIBS(target, name, value, (PropertyAttribute)None)

#define SET_FUNCTION_MEMBER(target, name, value) \
  SET_MEMBER( \
      target, name, v8::FunctionTemplate::New(value)->GetFunction())

#endif
