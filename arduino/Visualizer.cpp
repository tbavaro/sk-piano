#include "Visualizer.h"

Visualizer::Visualizer(LPD8806* strip) : strip(strip) {
}

void Visualizer::reset() {
  strip->reset();
  strip->show();
}
