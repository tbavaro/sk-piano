#include "WrappedSpi.h"
#include "WrapUtils.h"
#include "RuntimeException.h"

#include <v8.h>

#include <node_buffer.h>
#include <node_object_wrap.h>

using namespace node;
using namespace std;
using namespace v8;

WrappedSpi::WrappedSpi(const char* spiDevice, uint32_t maxSpeedHz)
    : Spi(spiDevice, maxSpeedHz) {}

WrappedSpi::~WrappedSpi() {}

Handle<Value> WrappedSpi::wrappedNew(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  // read spiDevice argument
  if (argc < 1 || !args[0]->IsString()) {
    return WrapUtils::makeErrorValue("spiDevice required");
  }
  const Handle<Value>& spiDeviceArg = args[0];
  String::AsciiValue spiDevice(spiDeviceArg);

  // read maxSpeedHz argument
  if (argc < 2 || !args[1]->IsNumber()) {
    return WrapUtils::makeErrorValue("maxSpeedHz required");
  }
  const Handle<Value>& maxSpeedHzArg = args[1];
  uint32_t maxSpeedHz = maxSpeedHzArg->Uint32Value();

  WrappedSpi* instance;
  RETURN_EXCEPTIONS_AS_NODE_ERRORS({
    instance = new WrappedSpi(*spiDevice, maxSpeedHz);
  });
  instance->Wrap(args.This());
  return args.This();
}

Handle<Value> WrappedSpi::wrappedClose(const Arguments& args) {
  HandleScope scope;
  CALL_WRAPPED_METHOD(WrappedSpi, close);
  return Undefined();
}

Handle<Value> WrappedSpi::wrappedSend(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  if (argc < 1 || !Buffer::HasInstance(args[0])) {
    return WrapUtils::makeErrorValue("buffer required");
  }
  const Handle<Value>& bufferArg = args[0];
  size_t bufferLength = Buffer::Length(bufferArg);

  CALL_WRAPPED_METHOD(WrappedSpi, send, Buffer::Data(bufferArg), bufferLength);
  return Undefined();
}

Handle<Value> WrappedSpi::init() {
  static Persistent<FunctionTemplate> persistentFunctionTemplate;
  static bool initialized = false;

  if (!initialized) {
    // see http://syskall.com/how-to-write-your-own-native-nodejs-extension/index.html/
    Local<FunctionTemplate> localFunctionTemplate =
        FunctionTemplate::New(WrappedSpi::wrappedNew);
    persistentFunctionTemplate =
        Persistent<FunctionTemplate>::New(localFunctionTemplate);
    persistentFunctionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    persistentFunctionTemplate->SetClassName(String::NewSymbol("Spi"));

    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "close", &WrappedSpi::wrappedClose);
    NODE_SET_PROTOTYPE_METHOD(persistentFunctionTemplate,
        "send", &WrappedSpi::wrappedSend);

    initialized = true;
  }

  return persistentFunctionTemplate->GetFunction();
}
