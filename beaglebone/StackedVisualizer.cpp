#include "StackedVisualizer.h"
#include "Colors.h"
#include "Util.h"
#include <string.h>

StackedVisualizer::StackedVisualizer(LightStrip& strip) : LightStripVisualizer(strip) {
  pixels = new Color[strip.numPixels()];
  age = strip.numPixels();
}

StackedVisualizer::~StackedVisualizer() {
  delete[] pixels;
}

void StackedVisualizer::onKeyDown(Key key) {
  Color c = Colors::hsv(360/88*key, 1.0, 1.0);
  pixels[0] = Colors::add(pixels[0], c); 
}

void StackedVisualizer::onKeyUp(Key key) {
}
  
void StackedVisualizer::onPassFinished(bool something_changed) {
  for (int fps = 0; fps < 5; ++fps) {
    if (Colors::BLACK != pixels[strip.numPixels() - 1]) {
      age--;
    }
    if (age == 0) {
      pixels[strip.numPixels() - 1] = Colors::BLACK;
      age = strip.numPixels();
    }
    for (int i = strip.numPixels() - 1; i > 0; --i) {
      if (Colors::BLACK == pixels[i]) {
        pixels[i] = pixels[i-1];
        pixels[i-1] = Colors::BLACK;
  	  }
      strip.setPixel(i, pixels[i]);
    }    
  }
}

void StackedVisualizer::reset() {
  LightStripVisualizer::reset();
  bzero(pixels, strip.numPixels() * sizeof(Color));
}

