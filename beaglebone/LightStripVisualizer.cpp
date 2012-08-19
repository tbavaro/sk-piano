#include "LightStripVisualizer.h"

LightStripVisualizer::LightStripVisualizer(LightStrip& strip) : strip(strip) {
}

void LightStripVisualizer::reset() {
  Visualizer::reset();
  strip.reset();
}
