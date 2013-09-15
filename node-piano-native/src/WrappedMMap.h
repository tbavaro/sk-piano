#ifndef __INCLUDED_WRAPPED_MMAP_H
#define __INCLUDED_WRAPPED_MMAP_H

#include <v8.h>

class WrappedMMap {
  public:
    static void init(v8::Handle<v8::Object> target);

    static v8::Handle<v8::Value> init();

  private:
    WrappedMMap(); // static only
};

#endif
