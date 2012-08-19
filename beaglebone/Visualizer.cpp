#include "Visualizer.h"

Visualizer::Visualizer(LightStrip& strip) : strip(strip) {
}

void Visualizer::reset() {
  strip.reset();
//  strip.show();
}
