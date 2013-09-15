#ifndef __INCLUDED_WRAPPED_SPI_H
#define __INCLUDED_WRAPPED_SPI_H

#include "Spi.h"

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

class WrappedSpi : public Spi, node::ObjectWrap {
  public:
    static v8::Handle<v8::Value> init();

  private:
    WrappedSpi(const char* spiDevice, uint32_t maxSpeedHz);
    ~WrappedSpi();

    static v8::Handle<v8::Value> wrappedNew(const v8::Arguments& args);
    static v8::Handle<v8::Value> wrappedClose(const v8::Arguments& args);
    static v8::Handle<v8::Value> wrappedSend(const v8::Arguments& args);
};

#endif
