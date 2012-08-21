#ifndef __INCLUDED_FRAME_BUFFER_LIGHT_STRIP_H
#define __INCLUDED_FRAME_BUFFER_LIGHT_STRIP_H

#include "BeagleBone.h"
#include "LightStrip.h"

class FrameBufferLightStrip : public LightStrip {
  public:
    FrameBufferLightStrip(int num_pixels);
    ~FrameBufferLightStrip();

    virtual int numPixels();
    virtual void reset();
    virtual void addPixel(Pixel n, Color color);
    virtual void setPixel(Pixel n, Color color);

  protected:
    Color getPixel(Pixel n);
    uint8_t* pixels;
    int num_pixels;
};

#endif

