#ifndef __INCLUDED_UTIL_H
#define __INCLUDED_UTIL_H

#include <stdint.h>

class Util {
  public:
    static void log(const char* fmt, ...);
    static void fatal(const char* fmt, ...);

    static uint32_t millis();
    static void delay(uint32_t millis);
    static void delay_until(uint32_t millis);
    static uint32_t random(uint32_t max);
    static float randomFloat(float max);
    static bool randomTest(float p);

    template <typename T> static T min(T a, T b);
    template <typename T> static T max(T a, T b);
};

template <typename T> inline T Util::min(T a, T b) {
  return (a < b) ? a : b;
}

template <typename T> inline T Util::max(T a, T b) {
  return (a > b) ? a : b;
}

#endif

