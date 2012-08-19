#include "PhysicalLightStrip.h"

#include <string.h>

PhysicalLightStrip::PhysicalLightStrip(SPI& spi, int num_pixels)
    : FrameBufferLightStrip(num_pixels), spi(spi) {
}

void PhysicalLightStrip::show() {
  spi.send(pixels, num_pixels * 3 + 1);
}

