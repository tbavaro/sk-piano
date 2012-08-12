#include "Util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void Util::fatal(const char* fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);
  fprintf(stderr, "FATAL: ");
  vfprintf(stderr, fmt, argptr);
  fprintf(stderr, "\n");
  va_end(argptr);
  exit(-1);
}

static uint32_t millis_raw() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
}

static struct timeval TV_START;

static int __init_tv_start() {
  gettimeofday(&TV_START, NULL);
}
static int __do_init_tv_start = __init_tv_start();

uint32_t Util::millis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return 
    (tv.tv_sec - TV_START.tv_sec) * 1000 + 
    (tv.tv_usec - TV_START.tv_usec) / 1000;
}

static uint32_t __init_millis = Util::millis();


