#include "tbbeaglebone.h"
#include "pin.h"
#include "wrapped_spi.h"

#include <node.h>

using namespace v8;

void TBBeagleBone::initModule(
    v8::Handle<v8::Object> exports,
    v8::Handle<v8::Object> module) {
  HandleScope scope;

  Pin::test(); //xcxc

  exports->Set(
      String::NewSymbol("Spi"),
      WrappedSpi::constructorFunctionTemplate()->GetFunction());
}

NODE_MODULE(tbbeaglebone, TBBeagleBone::initModule);
