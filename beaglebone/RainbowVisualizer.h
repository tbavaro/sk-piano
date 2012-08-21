#ifndef __INCLUDED_RAINBOW_VISUALIZER_H
#define __INCLUDED_RAINBOW_VISUALIZER_H

#include "LightStripVisualizer.h"

class RainbowVisualizer : public LightStripVisualizer {
  public:
    RainbowVisualizer(
        LightStrip& strip,
        float cycle_length,
        float flow_rate);
    
    virtual void onPassFinished(bool something_changed);
    virtual void reset();

  private:
    const float cycle_length;
    const float flow_rate;
    float phase;
    uint32_t prev_frame_time;
};

#endif

