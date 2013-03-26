#include "AmplitudeGlowVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>

AmplitudeGlowVisualizer::AmplitudeGlowVisualizer(
    LightStrip& strip, float note_increase, 
    float decrease_rate, float max_value) 
    : AbstractAmplitudeVisualizer(
        strip, note_increase, decrease_rate, max_value) {
}

template <typename T>
static T max(T a, T b) {
  return (a < b) ? b : a;
}

template <typename T>
static T min(T a, T b) {
  return (a > b) ? b : a;
}

void AmplitudeGlowVisualizer::renderValue(float value) {
  float capped_value = min(value, 1.0f);
  Color c = Colors::hsv(60.0 * pow(capped_value, 4.0), 1.0f - pow(capped_value, 20.0), capped_value);
  for (int i = 0; i < strip.numPixels(); ++i) {
    strip.setPixel(i, c);
  }
}
