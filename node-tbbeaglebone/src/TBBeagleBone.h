#ifndef __INCLUDED_TBBEAGLEBONE_H
#define __INCLUDED_TBBEAGLEBONE_H

#include <v8.h>

class TBBeagleBone {
  public:
    static void initModule(
        v8::Handle<v8::Object> exports,
        v8::Handle<v8::Object> module);

  private:
    TBBeagleBone(); // static only
};

#endif
