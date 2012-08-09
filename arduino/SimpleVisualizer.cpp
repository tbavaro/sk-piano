#include "SimpleVisualizer.h"

SimpleVisualizer::SimpleVisualizer(LPD8806* _strip) : Visualizer(_strip) {
  strip = _strip;
}

void SimpleVisualizer::onKeyDown(int key) {
  strip->setPixelColor(key, 0x7f7f7f);
}

void SimpleVisualizer::onKeyUp(int key) {
  strip->setPixelColor(key, 0x000000);
}
  
void SimpleVisualizer::onPassFinished(bool something_changed) {
  strip->show();
}

