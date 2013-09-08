#ifndef __INCLUDED_WRAPPED_SPI_H
#define __INCLUDED_WRAPPED_SPI_H

#include "spi.h"

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

class WrappedSpi : public Spi, node::ObjectWrap {
  public:
    WrappedSpi(const char* spiDevice, uint32_t maxSpeedHz);
    ~WrappedSpi();

    static v8::Handle<v8::Value> wrappedNew(const v8::Arguments& args);
};

#endif
