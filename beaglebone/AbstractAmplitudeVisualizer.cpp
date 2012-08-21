#include "AbstractAmplitudeVisualizer.h"
#include "Util.h"

#include <math.h>

AbstractAmplitudeVisualizer::AbstractAmplitudeVisualizer(
    LightStrip& strip, float note_increase, 
    float decrease_rate, float max_value) 
    : LightStripVisualizer(strip), 
      note_increase(note_increase),
      decrease_rate(decrease_rate),
      max_value(max_value),
      prev_frame_time(0),
      value(0) {
}

void AbstractAmplitudeVisualizer::onKeyDown(Key key) {
  value += note_increase;
  if (value > max_value) {
    value = max_value;
  }
}

void AbstractAmplitudeVisualizer::onPassFinished(bool something_changed) {
  uint32_t now = Util::millis();
  if (prev_frame_time != 0) {
    float secs = (now - prev_frame_time) / 1000.0;
    value -= secs * decrease_rate;
    if (value < 0) {
      value = 0;
    }
  }
  prev_frame_time = now;

  this->renderValue(value);
}

void AbstractAmplitudeVisualizer::reset() {
  prev_frame_time = 0;
  value = 0;
  this->renderValue(0);
}
