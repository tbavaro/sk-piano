#ifndef __INCLUDED_UTIL_H
#define __INCLUDED_UTIL_H

#include <stdint.h>

class Util {
  public:
    static void fatal(const char* fmt, ...);

    static uint32_t millis();
};

#endif

