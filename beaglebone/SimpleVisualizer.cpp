#include "SimpleVisualizer.h"
#include "Colors.h"

SimpleVisualizer::SimpleVisualizer(LightStrip& strip) 
    : LightStripVisualizer(strip) {
}

void SimpleVisualizer::onKeyDown(Key key) {
  strip.setPixel(key, Colors::WHITE);
}

void SimpleVisualizer::onKeyUp(Key key) {
  strip.setPixel(key, 0x000000);
}
  
