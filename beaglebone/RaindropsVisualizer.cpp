#include "RaindropsVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>
#include <string.h>

RaindropsVisualizer::RaindropsVisualizer(LightStrip& strip) 
    : AbstractAmplitudeVisualizer(strip, 0.22, 0.6, 1.2) {
  pixels = new Color[strip.numPixels()];
  old_pixels = new Color[strip.numPixels()];
  this->reset();
}

RaindropsVisualizer::~RaindropsVisualizer() {
  delete[] pixels;
  delete[] old_pixels;
}

template <typename T>
static inline T min(T a, T b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

void RaindropsVisualizer::renderValue(float value) {
  if (value > 1.0) {
    value = 1.0;
  }

  float drop_rate = 0.01 + pow(value * 1.5, 3.0);
  if (drop_rate > 1.0) {
    drop_rate = 1.0;
  }

  float saturation = pow(value, 0.5);
  float brightness = min(1.0, 0.25 + pow(value, 0.25));
  float decay_rate = 1.0 - (value * 0.05);
  int radius = Util::random(5 * pow(1.0 - value, 0.8)) + 1;

  for (int i = 0; i < 7; ++i) {
    if (Util::randomTest(drop_rate)) {
      Pixel pos = Util::random(strip.numPixels());
      Color c = Colors::hsv(Util::random(6) * 60, saturation, brightness);

      for (int j = ((int)pos) - radius; j <= (((int)pos) + radius); ++j) {
        if (j >= 0 && j < strip.numPixels()) {
          old_pixels[j] = c;
        }
      }
    }
  }

  for (int i = 0; i < strip.numPixels(); ++i) {
    Color pixels_to_average[3];
    int num_pixels = 0;
    pixels_to_average[num_pixels++] = old_pixels[i];
    if (i > 0) {
      pixels_to_average[num_pixels++] = old_pixels[i - 1];
    }
    if (i < (strip.numPixels() - 1)) {
      pixels_to_average[num_pixels++] = old_pixels[i + 1];
    }
    pixels[i] = Colors::multiply(Colors::average(pixels_to_average, num_pixels), decay_rate);
    strip.setPixel(i, pixels[i]);
  }
  
  Color* tmp = old_pixels;
  old_pixels = pixels;
  pixels = tmp;
}

// void RaindropsVisualizer::onPassFinished(bool something_changed) {
//   for (int i = 0; i < strip.numPixels(); ++i) {
//     Color pixels_to_average[3];
//     int num_pixels = 0;
//     pixels_to_average[num_pixels++] = old_pixels[i];
//     if (i > 0) {
//       pixels_to_average[num_pixels++] = old_pixels[i - 1];
//     }
//     if (i < (strip.numPixels() - 1)) {
//       pixels_to_average[num_pixels++] = old_pixels[i + 1];
//     }
//     pixels[i] = Colors::multiply(Colors::average(pixels_to_average, num_pixels), 0.95);
//     strip.setPixel(i, pixels[i]);
//   }
//   
//   Color* tmp = old_pixels;
//   old_pixels = pixels;
//   pixels = tmp;
// }
// 

void RaindropsVisualizer::reset() {
  LightStripVisualizer::reset();
  bzero(pixels, strip.numPixels() * sizeof(Color));
  bzero(old_pixels, strip.numPixels() * sizeof(Color));
}

