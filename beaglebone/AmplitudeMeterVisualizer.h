#ifndef __INCLUDED_AMPLITUDE_METER_VISUALIZER_H
#define __INCLUDED_AMPLITUDE_METER_VISUALIZER_H

#include "LightStripVisualizer.h"

class AmplitudeMeterVisualizer : public LightStripVisualizer {
  public:
    AmplitudeMeterVisualizer(
        LightStrip& strip,
        float note_increase,
        float decrease_rate,
        float max_value);

    virtual void onKeyDown(Key key);
    virtual void onPassFinished(bool something_changed);
    virtual void reset();

  private:
    const float note_increase;
    const float decrease_rate;
    const float max_value;
    float value;
    uint32_t prev_frame_time;
};

#endif

