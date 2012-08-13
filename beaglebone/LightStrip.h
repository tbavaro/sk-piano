#ifndef __INCLUDED_LIGHT_STRIP_H
#define __INCLUDED_LIGHT_STRIP_H

#include "Colors.h"
#include "SKTypes.h"

class LightStrip {
  public:
    virtual int numPixels() = 0;
    virtual void reset() = 0;
    virtual void addPixel(Pixel n, Color color) = 0;
    virtual void setPixel(Pixel n, Color color) = 0;
    virtual void show() = 0;
};

#endif

