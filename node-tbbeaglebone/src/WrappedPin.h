#ifndef __INCLUDED_WRAPPED_PIN_H
#define __INCLUDED_WRAPPED_PIN_H

#include "SlowPin.h"

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

class WrappedPin : public SlowPin, node::ObjectWrap {
  public:
    static const v8::Persistent<v8::FunctionTemplate>&
        constructorFunctionTemplate();

  private:
    WrappedPin(const std::string& name, int number);
    ~WrappedPin();

    static v8::Handle<v8::Value> wrappedNew(const v8::Arguments& args);

    static v8::Handle<v8::Value> wrappedClose(const v8::Arguments& args);
    static v8::Handle<v8::Value> wrappedSetMode(const v8::Arguments& args);
    static v8::Handle<v8::Value> wrappedRead(const v8::Arguments& args);
    static v8::Handle<v8::Value> wrappedWrite(const v8::Arguments& args);
};

#endif
