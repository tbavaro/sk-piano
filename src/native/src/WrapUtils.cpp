#include "WrapUtils.h"

#include <stdarg.h>

using namespace std;
using namespace v8;

static inline Handle<Value> makeErrorValueImpl(const char* msg) {
  Local<String> msgString = String::New(msg);
  Local<Value> exception = Exception::Error(msgString);
  return ThrowException(exception);
}

Handle<Value> WrapUtils::makeErrorValue(const char* fmt, ...) {
  char msg[4096];

  va_list argptr;
  va_start(argptr, fmt);
  vsnprintf(msg, sizeof(msg), fmt, argptr);
  va_end(argptr);

  return makeErrorValueImpl(msg);
}

Handle<Value> WrapUtils::makeErrorValue(const exception& e) {
  return makeErrorValueImpl(e.what());
}

Persistent<FunctionTemplate> WrapUtils::wrapConstructor(
    const char* className, v8::InvocationCallback newFunc) {
  // see http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
  Local<FunctionTemplate> lft = FunctionTemplate::New(newFunc);
  Persistent<FunctionTemplate>result = Persistent<FunctionTemplate>::New(lft);
  result->InstanceTemplate()->SetInternalFieldCount(1);
  result->SetClassName(String::NewSymbol(className));

  return result;
}