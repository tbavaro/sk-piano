#include "SimulatorLightStrip.h"
#include <stdio.h>
#include <stdlib.h>

SimulatorLightStrip::SimulatorLightStrip(int num_pixels)
    : FrameBufferLightStrip(num_pixels) {
}

SimulatorLightStrip::~SimulatorLightStrip() {
  fprintf(stderr, "bye\n");
  fflush(stderr);
  exit(1);
}

template<typename T> static inline T max(T a, T b) {
  return a > b ? a : b;
}

static inline uint8_t fix(uint8_t raw_value) {
  return (raw_value & 0x7f) * 2;
}

void SimulatorLightStrip::show() {
  printf("SHOW:[");
  for (int i = 0; i < num_pixels; ++i) {
    if (i > 0) {
      printf(",");
    }
    printf("'#%02x%02x%02x'", fix(pixels[i * 3 + 1]), fix(pixels[i * 3]), fix(pixels[i * 3 + 2]));
  }
  printf("]\n");
  fflush(stdout);
}

