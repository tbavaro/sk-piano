#include "Visualizer.h"

Visualizer::Visualizer(LPD8806* _strip) {
  strip = _strip;
}

void Visualizer::reset() {
  strip->reset();
  strip->show();
}
