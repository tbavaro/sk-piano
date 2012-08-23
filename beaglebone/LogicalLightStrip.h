#ifndef __DEFINED_LOGICAL_LIGHT_STRIP_H
#define __DEFINED_LOGICAL_LIGHT_STRIP_H

#include "LightStrip.h"
#include "SKTypes.h"

#include <utility>
#include <vector>

typedef std::pair<Pixel, Pixel> PixelRange;

class LogicalLightStrip : public LightStrip {
  public:
    static LogicalLightStrip* fromRange(
        LightStrip& delegate, 
        Pixel first_pixel, 
        Pixel last_pixel);
    static LogicalLightStrip* fromRanges(
        LightStrip& delegate,
        const std::vector<PixelRange>& ranges);
    
  public:
    virtual int numPixels();
    virtual void reset();
    virtual void addPixel(Pixel n, Color color);
    virtual void setPixel(Pixel n, Color color);
    virtual void show();

  protected:
    LogicalLightStrip(
        LightStrip& delegate, Pixel* pixel_mapping, int num_pixels);
    ~LogicalLightStrip();
    LightStrip& delegate;
    int num_pixels;
    Pixel* pixel_mapping;
};

#endif
