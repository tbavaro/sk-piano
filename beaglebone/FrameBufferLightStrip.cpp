#include "FrameBufferLightStrip.h"

#include <string.h>

FrameBufferLightStrip::FrameBufferLightStrip(int num_pixels)
    : num_pixels(num_pixels) {
  pixels = new uint8_t[num_pixels * 3 + 1];
  pixels[num_pixels * 3] = 0x00;
  this->reset();
}

FrameBufferLightStrip::~FrameBufferLightStrip() {
  delete[] pixels;
}

void FrameBufferLightStrip::reset() {
  memset(pixels, 0x80, num_pixels * 3);
}

int FrameBufferLightStrip::numPixels() {
  return num_pixels;
}

Color FrameBufferLightStrip::getPixel(Pixel n) {
  if (n >= num_pixels) {
    return Colors::BLACK;
  } else {
    return Colors::rgb(
        0x7f & pixels[n * 3 + 1], 
        0x7f & pixels[n * 3], 
        0x7f & pixels[n * 3 + 2]);
  }
}

void FrameBufferLightStrip::addPixel(Pixel n, Color c) {
  this->setPixel(n, Colors::add(this->getPixel(n), c));
}

void FrameBufferLightStrip::setPixel(Pixel n, Color c) {
  if (n < num_pixels) {
    uint8_t* p = &pixels[n * 3];
    *p++ = (c >> 16) | 0x80;
    *p++ = (c >> 8)  | 0x80;
    *p++ =  c        | 0x80;
  }
}

