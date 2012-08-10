#include "Colors.h"
#include <math.h>

Color Colors::cached_rainbow[360];

const Color Colors::WHITE = 0x7f7f7f;

void Colors::init() {
  // initialize rainbow
  for (int i = 0; i < 360; ++i) {
    cached_rainbow[i] = Colors::hsv(i, 1.0, 1.0);
  }
}

Color Colors::hsv(float h, float s, float v) {
  float r, g, b;
  if( s == 0 ) {
    // achromatic (grey)
    r = g = b = v;
  } else {
    h /= 60;                      // sector 0 to 5
    int i = floor(h);
    float f = h - i;                      // factorial part of h
    float p = v * (1 - s);
    float q = v * (1 - s * f);
    float t = v * (1 - s * (1 - f));
    switch(i) {
      case 0:
        r = v;
        g = t;
        b = p;
        break;
      case 1:
        r = q;
        g = v;
        b = p;
        break;
      case 2:
        r = p;
        g = v;
        b = t;
        break;
      case 3:
        r = p;
        g = q;
        b = v;
        break;
      case 4:
        r = t;
        g = p;
        b = v;
        break;
      default:                // case 5:
        r = v;
        g = p;
        b = q;
        break;
    }
  }
  return Colors::rgb(r * 127, g * 127, b * 127);
}
