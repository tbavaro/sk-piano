#include "LogicalLightStrip.h"
#include "Util.h"

using namespace std;

static const Pixel NOOP_PIXEL = 2000;

static int numPixelsInRange(Pixel first_pixel, Pixel last_pixel) {
  if (first_pixel < last_pixel) {
    return (last_pixel - first_pixel) + 1;
  } else {
    return (first_pixel - last_pixel) + 1;
  }
}

// returns the # of pixels added
static int addPixelRange(Pixel* pixels_out, Pixel first_pixel, Pixel last_pixel) {
  int direction = (first_pixel < last_pixel) ? 1 : -1;
  int num_pixels = 0;
  for (int i = first_pixel; i != last_pixel + direction; i += direction) {
    pixels_out[num_pixels++] = i;
  }
  return num_pixels;
}

LogicalLightStrip* LogicalLightStrip::fromRange(
    LightStrip& delegate,
    Pixel first_pixel,
    Pixel last_pixel) {
  vector<PixelRange> ranges(1, PixelRange(first_pixel, last_pixel));
  return LogicalLightStrip::fromRanges(delegate, ranges);
}

LogicalLightStrip* LogicalLightStrip::fromRanges(
    LightStrip& delegate,
    const vector<PixelRange>& ranges) {
  int num_pixels = 0;
  for (vector<PixelRange>::const_iterator i = ranges.begin();
       i != ranges.end(); ++i) {
    num_pixels += numPixelsInRange(i->first, i->second);
  }

  Pixel* pixels = new Pixel[num_pixels];
  Pixel* ptr = pixels;
  for (vector<PixelRange>::const_iterator i = ranges.begin();
       i != ranges.end(); ++i) {
    int num_added = addPixelRange(ptr, i->first, i->second);
    ptr += num_added;
  }

  return new LogicalLightStrip(delegate, pixels, num_pixels); 
}

LogicalLightStrip* LogicalLightStrip::noopStrip(
    LightStrip& delegate, int length) {
  Pixel* pixels = new Pixel[length];
  for (int i = 0; i < length; ++i) {
    pixels[i] = NOOP_PIXEL;
  }
  return new LogicalLightStrip(delegate, pixels, length);
}

LogicalLightStrip* LogicalLightStrip::joinLogicalStrips(
    LightStrip& delegate, const std::vector<LogicalLightStrip*> strips) {
  int total_length = 0;
  for (vector<LogicalLightStrip*>::const_iterator i = strips.begin();
       i != strips.end(); ++i) {
    total_length += (*i)->num_pixels;
  }

  Pixel* pixels = new Pixel[total_length];
  int n = 0;
  for (vector<LogicalLightStrip*>::const_iterator i = strips.begin();
       i != strips.end(); ++i) {
    Pixel* strip_pixels = (*i)->pixel_mapping;
    for (int j = 0; j < (*i)->num_pixels; ++j) {
      pixels[n++] = strip_pixels[j];
    }
  }
  return new LogicalLightStrip(delegate, pixels, total_length);
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

