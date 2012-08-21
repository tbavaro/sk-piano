#include "AmplitudeMeterVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>

AmplitudeMeterVisualizer::AmplitudeMeterVisualizer(
    LightStrip& strip, float note_increase, 
    float decrease_rate, float max_value) 
    : LightStripVisualizer(strip), 
      note_increase(note_increase),
      decrease_rate(decrease_rate),
      max_value(max_value),
      prev_frame_time(0),
      value(0) {
}

void AmplitudeMeterVisualizer::onKeyDown(Key key) {
  value += note_increase;
  if (value > max_value) {
    value = max_value;
  }
}

void AmplitudeMeterVisualizer::onPassFinished(bool something_changed) {
  uint32_t now = Util::millis();
  if (prev_frame_time != 0) {
    float secs = (now - prev_frame_time) / 1000.0;
    value -= secs * decrease_rate;
    if (value < 0) {
      value = 0;
    }
  }
  prev_frame_time = now;

  int total_pixels = strip.numPixels();
  int lit_pixels = total_pixels * value;
  for (int i = 0; i < strip.numPixels(); ++i) {
    Color c = (i < lit_pixels) ? Colors::WHITE : Colors::BLACK;
    strip.setPixel(i, c);
  }
}

void AmplitudeMeterVisualizer::reset() {
  LightStripVisualizer::reset();
  prev_frame_time = 0;
  value = 0;
}
