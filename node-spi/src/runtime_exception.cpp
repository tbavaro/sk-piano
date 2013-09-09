#include "runtime_exception.h"

#include <stdarg.h>
#include <stdio.h>

using namespace std;

runtime_error RuntimeException(const char* fmt, ...) {
  char buffer[4096];

  va_list argptr;
  va_start(argptr, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, argptr);
  va_end(argptr);

  return runtime_error(buffer);
}
