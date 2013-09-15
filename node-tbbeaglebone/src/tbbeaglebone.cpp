#include "TBBeagleBone.h"
#include "WrappedPin.h"
#include "WrappedPinScanner.h"
#include "WrappedSpi.h"

//xcxc
#include "MmapGpio.h"

#include <node.h>

using namespace v8;

void TBBeagleBone::initModule(
    v8::Handle<v8::Object> exports,
    v8::Handle<v8::Object> module) {
  HandleScope scope;

  exports->Set(
      String::NewSymbol("Pin"),
      WrappedPin::constructorFunctionTemplate()->GetFunction());

  exports->Set(
      String::NewSymbol("PinScanner"),
      WrappedPinScanner::constructorFunctionTemplate()->GetFunction());

  exports->Set(
      String::NewSymbol("Spi"),
      WrappedSpi::constructorFunctionTemplate()->GetFunction());

  //xcxc
  MmapGpio::test();
}

NODE_MODULE(tbbeaglebone, TBBeagleBone::initModule);
