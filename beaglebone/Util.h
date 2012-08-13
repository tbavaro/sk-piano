#ifndef __INCLUDED_UTIL_H
#define __INCLUDED_UTIL_H

#include <stdint.h>

class Util {
  public:
    static void fatal(const char* fmt, ...);

    static uint32_t millis();
    static void delay(uint32_t millis);
    static void delay_until(uint32_t millis);
    static uint32_t random(uint32_t max);
};

#endif

