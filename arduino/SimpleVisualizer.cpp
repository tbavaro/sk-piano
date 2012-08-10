#include "SimpleVisualizer.h"
#include "Colors.h"

SimpleVisualizer::SimpleVisualizer(LPD8806* strip) : Visualizer(strip) {
}

void SimpleVisualizer::onKeyDown(Key key) {
  strip->setPixelColor(key, Colors::WHITE);
}

void SimpleVisualizer::onKeyUp(Key key) {
  strip->setPixelColor(key, 0x000000);
}
  
void SimpleVisualizer::onPassFinished(bool something_changed) {
  strip->show();
}

