#include "wrapped_spi.h"
#include "runtime_exception.h"

#include <v8.h>

#include <node_buffer.h>
#include <node_object_wrap.h>

using namespace node;
using namespace std;
using namespace v8;

static inline Handle<Value> NodeError(const char* msg) {
  Local<String> msgString = String::New(msg);
  Local<Value> exception = Exception::Error(msgString);
  return ThrowException(exception);
}

WrappedSpi::WrappedSpi(const char* spiDevice, uint32_t maxSpeedHz)
    : Spi(spiDevice, maxSpeedHz) {}

WrappedSpi::~WrappedSpi() {}

#define returnExceptionsAsNodeErrors(block) \
  try { \
    block \
  } catch (const exception& e) { \
    return NodeError(e.what()); \
  }

#define callMethod(method, methodArgs...) \
  { \
    WrappedSpi* instance = ObjectWrap::Unwrap<WrappedSpi>(args.This()); \
    returnExceptionsAsNodeErrors({ \
      instance->method(methodArgs); \
    }) \
  }

#define callMethodWithReturn(returnVar, method, methodArgs...) \
  { \
    WrappedSpi* instance = ObjectWrap::Unwrap<WrappedSpi>(args.This()); \
    returnExceptionsAsNodeErrors({ \
      returnVar = instance->method(methodArgs); \
    }) \
  }

Handle<Value> WrappedSpi::wrappedNew(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  // read spiDevice argument
  if (argc < 1 || !args[0]->IsString()) {
    return NodeError("spiDevice required");
  }
  const Handle<Value>& spiDeviceArg = args[0];
  String::AsciiValue spiDevice(spiDeviceArg);

  // read maxSpeedHz argument
  if (argc < 2 || !args[1]->IsNumber()) {
    return NodeError("maxSpeedHz required");
  }
  const Handle<Value>& maxSpeedHzArg = args[1];
  uint32_t maxSpeedHz = maxSpeedHzArg->Uint32Value();

  WrappedSpi* instance;
  returnExceptionsAsNodeErrors({
    instance = new WrappedSpi(*spiDevice, maxSpeedHz);
  });
  instance->Wrap(args.This());
  return args.This();
}

Handle<Value> WrappedSpi::wrappedClose(const Arguments& args) {
  HandleScope scope;
  callMethod(close);
  return Undefined();
}

Handle<Value> WrappedSpi::wrappedSend(const Arguments& args) {
  HandleScope scope;

  int argc = args.Length();

  if (argc < 1 || !Buffer::HasInstance(args[0])) {
    return NodeError("buffer required");
  }
  const Handle<Value>& bufferArg = args[0];
  size_t bufferLength = Buffer::Length(bufferArg);

  callMethod(send, Buffer::Data(bufferArg), bufferLength);
  return Undefined();
}

const Persistent<FunctionTemplate>& WrappedSpi::constructorFunctionTemplate() {
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
  }

  return persistentFunctionTemplate;
}
