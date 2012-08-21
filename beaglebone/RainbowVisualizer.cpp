#include "RainbowVisualizer.h"
#include "Colors.h"
#include "Util.h"

#include <math.h>

RainbowVisualizer::RainbowVisualizer(
    LightStrip& strip, float cycle_length, float flow_rate) 
    : LightStripVisualizer(strip), 
      cycle_length(cycle_length), 
      flow_rate(flow_rate),
      prev_frame_time(0) {
  phase = 0;
}

static inline float niceMod(float x, float y) {
  float m = fmod(x, y);
  if (m < 0) {
    m += y;
  }
  return m;
}

void RainbowVisualizer::onPassFinished(bool something_changed) {
  uint32_t now = Util::millis();
  if (prev_frame_time != 0) {
    float secs = (now - prev_frame_time) / 1000.0;
    phase = niceMod(phase + flow_rate * secs, cycle_length);
  }
  prev_frame_time = now;

  for (int i = 0; i < strip.numPixels(); ++i) {
    int v = fmod((i + phase) / cycle_length * 360.0, 360.0);
    strip.setPixel(i, Colors::rainbow(v));
  }
}

void RainbowVisualizer::reset() {
  LightStripVisualizer::reset();
  prev_frame_time = 0;
  phase = 0;
}
