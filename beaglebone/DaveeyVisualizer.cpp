#include "DaveeyVisualizer.h"
#include "Colors.h"
#include "Util.h"
#include <string.h>

DaveeyVisualizer::DaveeyVisualizer(LightStrip& strip) : LightStripVisualizer(strip) {
  pixels = new Color[strip.numPixels()];
  old_pixels = new Color[strip.numPixels()];
}

DaveeyVisualizer::~DaveeyVisualizer() {
  delete[] pixels;
  delete[] old_pixels;
}

int DaveeyVisualizer::pixelForKey(Key key) {
  return ((int)key / 88.0 * strip.numPixels());
}

void DaveeyVisualizer::onKeyDown(Key key) {
  Color c = Colors::hsv(Util::random(360), 1.0, 1.0);
  old_pixels[pixelForKey(key)] =
      Colors::add(old_pixels[pixelForKey(key)], c); 
}

void DaveeyVisualizer::onKeyUp(Key key) {
}
  
void DaveeyVisualizer::onPassFinished(bool something_changed) {
  for (int i = 0; i < strip.numPixels(); ++i) {
    Color pixels_to_average[3];
    int num_pixels = 0;
    pixels_to_average[num_pixels++] = old_pixels[i];
    if (i > 0) {
      pixels_to_average[num_pixels++] = old_pixels[i - 1];
    }
    if (i < (strip.numPixels() - 1)) {
      pixels_to_average[num_pixels++] = old_pixels[i + 1];
    }
    pixels[i] = Colors::multiply(Colors::average(pixels_to_average, num_pixels), 0.95);
    strip.setPixel(i, pixels[i]);
  }
  
  Color* tmp = old_pixels;
  old_pixels = pixels;
  pixels = tmp;
}

void DaveeyVisualizer::reset() {
  LightStripVisualizer::reset();
  bzero(pixels, strip.numPixels() * sizeof(Color));
  bzero(old_pixels, strip.numPixels() * sizeof(Color));
}

