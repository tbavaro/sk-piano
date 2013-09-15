#include "NodeModule.h"
#include "WrappedMMap.h"
#include "WrappedSpi.h"

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

void NodeModule::initModule(
    v8::Handle<v8::Object> exports,
    v8::Handle<v8::Object> module) {
  HandleScope scope;

  exports->Set(String::NewSymbol("Spi"), WrappedSpi::init());
  exports->Set(String::NewSymbol("MMap"), WrappedMMap::init());

  test();
}

NODE_MODULE(piano_native, NodeModule::initModule);
