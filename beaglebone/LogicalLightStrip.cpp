#include "LogicalLightStrip.h"
#include "Util.h"

LogicalLightStrip* LogicalLightStrip::fromRange(
    LightStrip& delegate,
    Pixel first_pixel,
    Pixel last_pixel) {
  int direction = (first_pixel < last_pixel) ? 1 : -1;
  int num_pixels;
  if (direction == 1) {
    num_pixels = (last_pixel - first_pixel) + 1;
  } else {
    num_pixels = (first_pixel - last_pixel) + 1;
  }

  Pixel* pixel_mapping = new Pixel[num_pixels];
  for (int i = 0; i < num_pixels; ++i) {
    pixel_mapping[i] = first_pixel + i * direction;
    Util::log("%d -> %d", i, pixel_mapping[i]);
  }

  return new LogicalLightStrip(delegate, pixel_mapping, num_pixels); 
}

LogicalLightStrip::LogicalLightStrip(
    LightStrip& delegate, Pixel* pixel_mapping, int num_pixels)
        : delegate(delegate),
          pixel_mapping(pixel_mapping),
          num_pixels(num_pixels) {
}

LogicalLightStrip::~LogicalLightStrip() {
  delete[] pixel_mapping;
}

int LogicalLightStrip::numPixels() {
  return num_pixels;
}

void LogicalLightStrip::reset() {
  for (int i = 0; i < num_pixels; ++i) {
    delegate.setPixel(pixel_mapping[i], Colors::BLACK);
  }
}

void LogicalLightStrip::addPixel(Pixel n, Color color) {
  if (n < num_pixels) {
    delegate.addPixel(pixel_mapping[n], color);
  }
}

void LogicalLightStrip::setPixel(Pixel n, Color color) {
  if (n < num_pixels) {
    delegate.setPixel(pixel_mapping[n], color);
  }
}

void LogicalLightStrip::show() {
  Util::fatal("can't call show() on LogicalLightStrip");
}

