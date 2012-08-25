#include "TwinkleVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>
#include <strings.h>

TwinkleVisualizer::TwinkleVisualizer(LightStrip& strip) 
  : AbstractAmplitudeVisualizer(strip, 0.15, 0.3, 1.2),
    pixel_values(new float[strip.numPixels()]) {
  bzero(pixel_values, strip.numPixels() * sizeof(float));
}

TwinkleVisualizer::~TwinkleVisualizer() {
  delete[] pixel_values;
}

int TwinkleVisualizer::pixelForKey(Key key) {
  return ((int)key / 88.0 * strip.numPixels());
}

void TwinkleVisualizer::onKeyDown(Key key) {
  AbstractAmplitudeVisualizer::onKeyDown(key);
//  int pixel = pixelForKey(key);
//  strip.setPixel(pixel, colorForCount(++pixel_counts[pixel]));
}

void TwinkleVisualizer::onKeyUp(Key key) {
  AbstractAmplitudeVisualizer::onKeyUp(key);
//  int pixel = pixelForKey(key);
//  strip.setPixel(pixel, colorForCount(--pixel_counts[pixel]));
}
  
void TwinkleVisualizer::renderValue(float value) {
  value = Util::min(1.0f, value);

  // decay pixels
  for (int i = 0; i < strip.numPixels(); ++i) {
//    pixel_values[i] = Util::max(0.0, pixel_values[i] - 0.05);
//    pixel_values[i] = Util::max(0.0, pixel_values[i] * 0.95 - 0.01);
    pixel_values[i] *= 0.8;
  }

  // reset some pixels to 1
  float rate = 0.00005 + (pow(value, 0.5) * 0.05);
  for (int i = 0; i < (strip.numPixels() * rate); ++i) {
    Pixel pixel = Util::random(strip.numPixels());
//    pixel_values[pixel] += 0.5;
    pixel_values[pixel] = 1.0;
  }

  // draw
  float saturation = Util::min(1.0, pow(value, 2.0));
  float hue = (Util::millis() / 30) % 360;
  for (int i = 0; i < strip.numPixels(); ++i) {
    float brightness = Util::min(1.0f, pixel_values[i]);
    strip.setPixel(i, Colors::hsv(hue, saturation, brightness));
  }
}

