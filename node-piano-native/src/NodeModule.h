#ifndef __INCLUDED_NODE_MODULE_H
#define __INCLUDED_NODE_MODULE_H

#include <v8.h>

class NodeModule {
  public:
    static void initModule(
        v8::Handle<v8::Object> exports,
        v8::Handle<v8::Object> module);

  private:
    NodeModule(); // static only
};

#endif
