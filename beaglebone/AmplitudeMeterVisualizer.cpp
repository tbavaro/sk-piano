#include "AmplitudeMeterVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>

AmplitudeMeterVisualizer::AmplitudeMeterVisualizer(
    LightStrip& strip, float note_increase, 
    float decrease_rate, float max_value) 
    : AbstractAmplitudeVisualizer(
        strip, note_increase, decrease_rate, max_value) {
}

void AmplitudeMeterVisualizer::renderValue(float value) {
  int total_pixels = strip.numPixels();
  int lit_pixels = total_pixels * value;
  for (int i = 0; i < strip.numPixels(); ++i) {
    Color c = (i < lit_pixels) ? Colors::WHITE : Colors::BLACK;
    strip.setPixel(i, c);
  }
}
