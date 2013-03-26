#include "IdleVisualizerWrapper.h"
#include "Util.h"
#include <math.h>
#include <strings.h>

IdleVisualizerWrapper::IdleVisualizerWrapper(
    Visualizer& delegate,
    float idle_timeout,
    float random_key_rate, 
    float min_press_time,
    float max_press_time)
        : delegate(delegate), 
          idle_timeout_ms(idle_timeout * 1000),
          random_key_rate(random_key_rate),
          key_up_times(new uint32_t[88]),
          min_press_ms(min_press_time * 1000),
          max_press_ms(max_press_time * 1000) {
  bzero(key_up_times, 88 * sizeof(uint32_t));
  prev_key_time = Util::millis();
}
    
void IdleVisualizerWrapper::reset() {
//  prev_key_time = Util::millis();
  delegate.reset();
}

void IdleVisualizerWrapper::onKeyDown(Key key) {
  prev_key_time = Util::millis();
  delegate.onKeyDown(key);
}

void IdleVisualizerWrapper::onKeyUp(Key key) {
  prev_key_time = Util::millis();
  delegate.onKeyUp(key);
}

void IdleVisualizerWrapper::onPassFinished(bool something_changed) {
  uint32_t now = Util::millis();
  for (int i = 0; i < 88; ++i) {
    if (key_up_times[i] > 0 && key_up_times[i] <= now) {
      delegate.onKeyUp(i);
      key_up_times[i] = 0;
    }
  }
  if (now - prev_key_time >= idle_timeout_ms) {
    if (Util::random(1000) < random_key_rate * 1000) {
      Key key = Util::random(88);
      if (key_up_times[key] != 0) {
        delegate.onKeyUp(key);
      }
      delegate.onKeyDown(key);
      key_up_times[key] = now + Util::random(max_press_ms - min_press_ms) + min_press_ms;
      something_changed = true;
    }
  }
  delegate.onPassFinished(something_changed);
}

