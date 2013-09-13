#include "WrappedPin.h"
#include "RuntimeException.h"
#include "WrapUtils.h"

using namespace node;
using namespace std;
using namespace v8;

WrappedPin::WrappedPin(const string& name, int number)
    : Pin(name, number) {}

WrappedPin::~WrappedPin() {}

Handle<Value> WrappedPin::wrappedNew(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  // read name argument
  if (argc < 1 || !args[0]->IsString()) {
    return WrapUtils::makeErrorValue("name required");
  }
  const Handle<Value>& nameArg = args[0];
  String::AsciiValue name(nameArg);

  // read number argument
  if (argc < 2 || !args[1]->IsNumber()) {
    return WrapUtils::makeErrorValue("number required");
  }
  const Handle<Value>& numberArg = args[1];
  uint32_t number = numberArg->Uint32Value();

  WrappedPin* instance;
  RETURN_EXCEPTIONS_AS_NODE_ERRORS({
    instance = new WrappedPin(*name, number);
  });
  instance->Wrap(args.This());
  return args.This();
}

Handle<Value> WrappedPin::wrappedClose(const Arguments& args) {
  HandleScope scope;
  CALL_WRAPPED_METHOD(WrappedPin, close);
  return Undefined();
}

Handle<Value> WrappedPin::wrappedSetMode(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  if (argc < 1 || !args[0]->IsNumber()) {
    return WrapUtils::makeErrorValue("mode required");
  }
  const Handle<Value>& modeArg = args[0];
  uint32_t modeInt = modeArg->Uint32Value();

  Pin::Mode mode;
  switch(modeInt) {
    case 0:   mode = Pin::INPUT;   break;
    case 1:   mode = Pin::OUTPUT;  break;
    default:
      return WrapUtils::makeErrorValue("invalid mode: %d", modeInt);
  }

  CALL_WRAPPED_METHOD(WrappedPin, setMode, mode);
  return Undefined();
}

Handle<Value> WrappedPin::wrappedRead(const Arguments& args) {
  HandleScope scope;
  bool result;
  CALL_WRAPPED_METHOD_WITH_RETURN(WrappedPin, result, read);

  return Boolean::New(result);
}

Handle<Value> WrappedPin::wrappedWrite(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  if (argc < 1 || !args[0]->IsBoolean()) {
    return WrapUtils::makeErrorValue("value required");
  }
  const Handle<Value>& valueArg = args[0];
  bool value = (valueArg->Uint32Value() != 0);

  CALL_WRAPPED_METHOD(WrappedPin, write, value);
  return Undefined();
}

const Persistent<FunctionTemplate>& WrappedPin::constructorFunctionTemplate() {
  static Persistent<FunctionTemplate> persistentFunctionTemplate;
  static bool initialized = false;

  if (!initialized) {
    // see http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
    Local<FunctionTemplate> localFunctionTemplate =
        FunctionTemplate::New(WrappedPin::wrappedNew);
    persistentFunctionTemplate =
        Persistent<FunctionTemplate>::New(localFunctionTemplate);
    persistentFunctionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    persistentFunctionTemplate->SetClassName(String::NewSymbol("Pin"));

    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "close", &WrappedPin::wrappedClose);
    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "setMode", &WrappedPin::wrappedSetMode);
    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "read", &WrappedPin::wrappedRead);
    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "write", &WrappedPin::wrappedWrite);

    initialized = true;
  }

  return persistentFunctionTemplate;
}
