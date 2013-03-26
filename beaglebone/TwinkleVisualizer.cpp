#include "TwinkleVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>
#include <strings.h>

TwinkleVisualizer::TwinkleVisualizer(LightStrip& strip, bool highlight_keys) 
  : AbstractAmplitudeVisualizer(strip, 0.15, 0.3, 1.2),
    pixel_values(new float[strip.numPixels()]),
    pixel_saturations(new float[strip.numPixels()]),
    keys(new bool[88]),
    sparkle_accum(0),
    highlight_keys(highlight_keys) {
  this->reset();
}

TwinkleVisualizer::~TwinkleVisualizer() {
  delete[] pixel_values;
  delete[] pixel_saturations;
  delete[] keys;
}

void TwinkleVisualizer::reset() {
  AbstractAmplitudeVisualizer::reset();
  bzero(pixel_values, strip.numPixels() * sizeof(float));
  bzero(pixel_saturations, strip.numPixels() * sizeof(float));
  bzero(keys, 88 * sizeof(bool));
  sparkle_accum = 0;
}

int TwinkleVisualizer::pixelForKey(Key key) {
  return ((int)key / 88.0 * strip.numPixels());
}

void TwinkleVisualizer::onKeyDown(Key key) {
  AbstractAmplitudeVisualizer::onKeyDown(key);
  keys[key] = true;
}

void TwinkleVisualizer::onKeyUp(Key key) {
  AbstractAmplitudeVisualizer::onKeyUp(key);
  keys[key] = false;
}
  
void TwinkleVisualizer::renderValue(float value) {
  value = Util::min(1.0f, value);

  // decay pixels
  for (int i = 0; i < strip.numPixels(); ++i) {
    pixel_values[i] *= 0.8;
  }

  // reset some pixels to 1
//  float rate = 0.005 + (pow(value, 0.8) * 0.005);
  sparkle_accum += strip.numPixels() * (0.001 + (pow(value, 0.8) * 0.05));
  for (int i = 0; i < ((int)sparkle_accum); ++i) {
    Pixel pixel = Util::random(strip.numPixels());
    pixel_values[pixel] = 1.0;
  }
  sparkle_accum = fmod(sparkle_accum, 1.0f);

  // decay saturations
  for (int i = 0; i < strip.numPixels(); ++i) {
    pixel_saturations[i] *= 0.9;
  }

  // set saturations for pressed keys
  int radius = strip.numPixels() / 88.0 * 2.0;
  for (int i = 0; i < 88; ++i) {
    if (keys[i]) {
      int pixel = pixelForKey(i);
      int left_pixel = Util::max(0, pixel - radius);
      int right_pixel = Util::min(strip.numPixels() - 1, pixel + radius);
      for (int j = left_pixel; j <= right_pixel; ++j) {
        pixel_saturations[j] = 1.0;
      }
    }
  }

  // draw
  float overall_saturation = Util::min(1.0, pow(value, 2.0));
  float hue = (Util::millis() / 30) % 360;
  for (int i = 0; i < strip.numPixels(); ++i) {
    float saturation;
    float brightness;
    if (highlight_keys) {
      saturation = Util::max(0.0f, overall_saturation - pixel_saturations[i]);
      brightness = Util::min(1.0f, pixel_values[i] * 0.3f + pixel_saturations[i] * 0.7f);
    } else {
      saturation = overall_saturation;
      brightness = Util::min(1.0f, pixel_values[i]);
    }
    strip.setPixel(i, Colors::hsv(hue, saturation, brightness));
  }
}

