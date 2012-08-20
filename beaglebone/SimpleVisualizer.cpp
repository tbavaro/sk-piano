#include "SimpleVisualizer.h"
#include "Colors.h"

SimpleVisualizer::SimpleVisualizer(LightStrip& strip) : LightStripVisualizer(strip) {
  pixel_counts = new int[strip.numPixels()];
}

SimpleVisualizer::~SimpleVisualizer() {
  delete[] pixel_counts;
}

int SimpleVisualizer::pixelForKey(Key key) {
  return ((int)key / 88.0 * strip.numPixels());
}

Color SimpleVisualizer::colorForCount(int count) {
  return (count > 0 ? Colors::WHITE : Colors::BLACK);
}

void SimpleVisualizer::onKeyDown(Key key) {
  int pixel = pixelForKey(key);
  strip.setPixel(pixel, colorForCount(++pixel_counts[pixel]));
}

void SimpleVisualizer::onKeyUp(Key key) {
  int pixel = pixelForKey(key);
  strip.setPixel(pixel, colorForCount(--pixel_counts[pixel]));
}
  
void SimpleVisualizer::onPassFinished(bool something_changed) {
//  strip.show();
}

