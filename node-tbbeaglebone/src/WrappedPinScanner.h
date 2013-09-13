#ifndef __INCLUDED_WRAPPED_PIN_SCANNER_H
#define __INCLUDED_WRAPPED_PIN_SCANNER_H

#include "PinScanner.h"

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>
#include <vector>

class WrappedPinScanner : public PinScanner, node::ObjectWrap {
  public:
    static const v8::Persistent<v8::FunctionTemplate>&
        constructorFunctionTemplate();

  private:
    WrappedPinScanner(
        const std::vector<v8::Persistent<v8::Value> >& outputPins,
        const std::vector<v8::Persistent<v8::Value> >& inputPins);
    ~WrappedPinScanner();

    std::vector<v8::Persistent<v8::Value> > outputPins;
    std::vector<v8::Persistent<v8::Value> > inputPins;

    static v8::Handle<v8::Value> wrappedNew(const v8::Arguments& args);

    static v8::Handle<v8::Value> wrappedScan(const v8::Arguments& args);
};

#endif
