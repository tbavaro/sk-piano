#include "LifeVisualizer.h"
#include "Colors.h"
#include "Util.h"
#include <string.h>

LifeVisualizer::LifeVisualizer(LightStrip& strip) : LightStripVisualizer(strip) {
  pixels = new Color[strip.numPixels()];
  old_pixels = new Color[strip.numPixels()];
}

LifeVisualizer::~LifeVisualizer() {
  delete[] pixels;
  delete[] old_pixels;
}

int LifeVisualizer::pixelForKey(Key key) {
  return ((int)key / 88.0 * strip.numPixels());
}

void LifeVisualizer::onKeyDown(Key key) {
  Color c = Colors::hsv(Util::random(360), 1.0, 1.0);
  Pixel pixel = pixelForKey(key);
  old_pixels[pixel] = c;
}

void LifeVisualizer::onKeyUp(Key key) {
}
  
void LifeVisualizer::onPassFinished(bool something_changed) {
  for (int i = 0; i < strip.numPixels(); ++i) {
    Color pixels_to_use[3];
    int num_pixels = 0;
    char state = 0;
    if (i > 0) {
      pixels_to_use[num_pixels++] = old_pixels[i - 1];
      if (old_pixels[i - 1] != Colors::BLACK) {
        state += 1;
      }
    }
    state *= 2;
    pixels_to_use[num_pixels++] = old_pixels[i];
    if (old_pixels[i] != Colors::BLACK) {
      state += 1;
    }
    state *= 2;
    if (i < (strip.numPixels() - 1)) {
      pixels_to_use[num_pixels++] = old_pixels[i + 1];
      if (old_pixels[i + 1] != Colors::BLACK) {
        state += 1;
      }
    }
    
    //char states[8] = {0,1,1,1,0,1,1,0}; 
    char states[8] = {0,1,1,0,0,1,1,0}; 
    if (states[state]) {
      pixels[i] = Colors::WHITE;
    } else {
      pixels[i] = Colors::BLACK;
    }
    strip.setPixel(i, pixels[i]);
  }
  
  Color* tmp = old_pixels;
  old_pixels = pixels;
  pixels = tmp;
}

void LifeVisualizer::reset() {
  LightStripVisualizer::reset();
  bzero(pixels, strip.numPixels() * sizeof(Color));
  bzero(old_pixels, strip.numPixels() * sizeof(Color));
}

