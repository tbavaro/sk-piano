#include "SimulatorLightStrip.h"
#include <stdio.h>

SimulatorLightStrip::SimulatorLightStrip(int num_pixels)
    : FrameBufferLightStrip(num_pixels) {
}

template<typename T> static inline T max(T a, T b) {
  return a > b ? a : b;
}

void SimulatorLightStrip::show() {
  char line[num_pixels + 1];
  line[num_pixels] = '\0';
  for (int i = 0; i < num_pixels; ++i) {
    uint8_t brightest = pixels[i * 3];
    brightest = max(brightest, pixels[i * 3 + 1]);
    brightest = max(brightest, pixels[i * 3 + 2]);
    brightest &= 0x7f;
    char c;
    if (brightest == 0) {
      c = ' ';
    } else if (brightest < 0x20) {
      c = '.';
    } else if (brightest < 0x40) {
      c = ',';
    } else if (brightest < 0x60) {
      c = 'x';
    } else {
      c = '#';
    }
    line[i] = c;
  }
  printf("\r%s", line);
  fflush(stdout);
}

