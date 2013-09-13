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
