#include "IdleVisualizerWrapper.h"
#include "Util.h"

IdleVisualizerWrapper::IdleVisualizerWrapper(
    Visualizer& delegate,
    float idle_timeout,
    float random_key_rate) 
        : delegate(delegate), 
          idle_timeout_ms(idle_timeout * 1000),
          random_key_rate(random_key_rate) {
  prev_key_time = Util::millis();
}

void IdleVisualizerWrapper::reset() {
  prev_key_time = Util::millis();
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
  if (now - prev_key_time >= idle_timeout_ms) {
    if (Util::random(1000) < random_key_rate * 1000) {
      Key key = Util::random(88);
      delegate.onKeyDown(key);
      delegate.onKeyUp(key);
      something_changed = true;
    }
  }
  delegate.onPassFinished(something_changed);
}

