#ifndef __INCLUDED_COLORS_H
#define __INCLUDED_COLORS_H

#include "stdint.h"

typedef int32_t Color;

class Colors {
  public:
    /**
     * Calculate color value based on r, g, b values from 0-127
     */
    static Color rgb(uint8_t r, uint8_t g, uint8_t b);

    /**
     * Slow calculation method to convert hue (0 - 360), saturation (0 - 1), 
     * and value (0 - 1) to an RGB color.  Can possibly make this faster using
     * only integer math but making lookup tables seems like a better approach.
     */
    static Color hsv(float h, float s, float v);

    /**
     * Returns a full-brightness, full-saturation color with the given hue.
     * This is much faster than calling hsv above on every pixel.
     */
    static Color rainbow(uint16_t hue);

    static const Color BLACK;
    static const Color WHITE;
};

inline Color Colors::rgb(uint8_t r, uint8_t g, uint8_t b) {
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

#endif

