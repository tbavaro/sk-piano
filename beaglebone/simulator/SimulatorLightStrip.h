#ifndef __INCLUDED_SIMULATOR_LIGHT_STRIP_H
#define __INCLUDED_SIMULATOR_LIGHT_STRIP_H

#include "FrameBufferLightStrip.h"

class SimulatorLightStrip : public FrameBufferLightStrip {
  public:
    SimulatorLightStrip(int num_pixels);
    ~SimulatorLightStrip();
    virtual void show();
};

#endif
