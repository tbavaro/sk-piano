#include "SimulatorLightStrip.h"
#include <stdio.h>
#include <stdlib.h>

SimulatorLightStrip::SimulatorLightStrip(int num_pixels)
    : FrameBufferLightStrip(num_pixels) {
}

SimulatorLightStrip::~SimulatorLightStrip() {
}

template<typename T> static inline T max(T a, T b) {
  return a > b ? a : b;
}

void SimulatorLightStrip::show() {
  printf("SHOW:[");
  for (int i = 0; i < num_pixels; ++i) {
    if (i > 0) {
      printf(",");
    }
    Color adj_color = Colors::rgb(pixels[i * 3 + 1], pixels[i * 3], pixels[i * 3 + 2]);
    adj_color = Colors::gammaCorrect(adj_color, 2.0);
    printf("'#%02x%02x%02x'", Colors::red(adj_color) * 2, Colors::green(adj_color) * 2, Colors::blue(adj_color) * 2);
  }
  printf("]\n");
  fflush(stdout);
}

