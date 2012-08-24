#include "RaindropsVisualizer.h"
#include "Colors.h"
#include "Util.h"
#include <string.h>

RaindropsVisualizer::RaindropsVisualizer(LightStrip& strip) 
    : AbstractAmplitudeVisualizer(strip, 0.2, 0.1, 1.5) {
  pixels = new Color[strip.numPixels()];
  old_pixels = new Color[strip.numPixels()];
}

RaindropsVisualizer::~RaindropsVisualizer() {
  delete[] pixels;
  delete[] old_pixels;
}

void RaindropsVisualizer::renderValue(float value) {
  float drop_rate = 0.1 + value * 0.9;
  
  //if (Util::random_test(drop_rate)) {
  //  Pixel pos = 0;
  //}
}

// void RaindropsVisualizer::onPassFinished(bool something_changed) {
//   for (int i = 0; i < strip.numPixels(); ++i) {
//     Color pixels_to_average[3];
//     int num_pixels = 0;
//     pixels_to_average[num_pixels++] = old_pixels[i];
//     if (i > 0) {
//       pixels_to_average[num_pixels++] = old_pixels[i - 1];
//     }
//     if (i < (strip.numPixels() - 1)) {
//       pixels_to_average[num_pixels++] = old_pixels[i + 1];
//     }
//     pixels[i] = Colors::multiply(Colors::average(pixels_to_average, num_pixels), 0.95);
//     strip.setPixel(i, pixels[i]);
//   }
//   
//   Color* tmp = old_pixels;
//   old_pixels = pixels;
//   pixels = tmp;
// }
// 
// void RaindropsVisualizer::reset() {
//   LightStripVisualizer::reset();
//   bzero(pixels, strip.numPixels() * sizeof(Color));
//   bzero(old_pixels, strip.numPixels() * sizeof(Color));
// }
// 
