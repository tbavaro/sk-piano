#include "TBBeagleBone.h"
#include "WrappedMmap.h"
#include "WrappedPin.h"
#include "WrappedPinScanner.h"
#include "WrappedSpi.h"

#include "MmapPin.h"

#include <node.h>
#include <unistd.h>

using namespace v8;

static void test() {
//  MmapPin pin(*GpioConfig::userLed(3));
//  pin.setMode(Pin::OUTPUT);
//
//  while(true) {
//    pin.write(false);
//    sleep(1);
//    pin.write(true);
//    sleep(1);
//  }
}

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

  WrappedMmap::init(exports);

  test();
}

NODE_MODULE(tbbeaglebone, TBBeagleBone::initModule);
