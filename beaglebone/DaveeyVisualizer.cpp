#include "DaveeyVisualizer.h"
#include "Colors.h"
#include "Util.h"
#include <string.h>

DaveeyVisualizer::DaveeyVisualizer(LightStrip& strip) : Visualizer(strip) {
  pixels = new Color[strip.numPixels()];
  old_pixels = new Color[strip.numPixels()];
  prev_frame_time = 0;
}

DaveeyVisualizer::~DaveeyVisualizer() {
  delete[] pixels;
  delete[] old_pixels;
}

int DaveeyVisualizer::pixelForKey(Key key) {
  return ((int)key / 88.0 * strip.numPixels());
}

void DaveeyVisualizer::onKeyDown(Key key) {
  old_pixels[pixelForKey(key)] = Colors::hsv(Util::random(360), 1.0, 1.0);
}

void DaveeyVisualizer::onKeyUp(Key key) {
}
  
void DaveeyVisualizer::onPassFinished(bool something_changed) {
  uint32_t now = Util::millis();
  if (prev_frame_time != 0) {
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
  }

  memcpy(old_pixels, pixels, strip.numPixels() * sizeof(Color));
  Util::delay(30);

  prev_frame_time = now;
}

