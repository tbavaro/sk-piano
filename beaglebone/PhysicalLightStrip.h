#ifndef __INCLUDED_PHYSICAL_LIGHT_STRIP_H
#define __INCLUDED_PHYSICAL_LIGHT_STRIP_H

#include "BeagleBone.h"
#include "FrameBufferLightStrip.h"

class PhysicalLightStrip : public FrameBufferLightStrip {
  public:
    PhysicalLightStrip(SPI& spi, int num_pixels);

    virtual void show();

  private:
    SPI& spi;
};

#endif

