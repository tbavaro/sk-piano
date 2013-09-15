#ifndef __INCLUDED_WRAPPED_MMAP_H
#define __INCLUDED_WRAPPED_MMAP_H

#include <v8.h>

class WrappedMmap {
  public:
    static void init(v8::Handle<v8::Object> target);

  private:
    WrappedMmap(); // static only
};

#endif
