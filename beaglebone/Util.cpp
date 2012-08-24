#include "Util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void Util::log(const char* fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);
  vfprintf(stdout, fmt, argptr);
  fprintf(stdout, "\n");
  va_end(argptr);
}

void Util::fatal(const char* fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);
  fprintf(stdout, "FATAL: ");
  vfprintf(stdout, fmt, argptr);
  fprintf(stdout, "\n");
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

void Util::delay(uint32_t millis) {
  timespec ts;
  ts.tv_sec = millis / 1000;
  ts.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&ts, NULL);
}

void Util::delay_until(uint32_t millis) {
  uint32_t now = Util::millis();
  if (now < millis) {
    Util::delay(millis - now);
  }
}

static uint32_t __init_millis = Util::millis();

uint32_t Util::random(uint32_t max) {
  if (max == 0) {
    return max;
  }
  return ::random() % max;
}

bool Util::randomTest(float p) {
  uint32_t threshold = p * RAND_MAX;
  return (::random() < threshold);
}
