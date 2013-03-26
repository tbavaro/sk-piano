#ifndef __INCLUDED_AMPLITUDE_METER_VISUALIZER_H
#define __INCLUDED_AMPLITUDE_METER_VISUALIZER_H

#include "AbstractAmplitudeVisualizer.h"

class AmplitudeMeterVisualizer : public AbstractAmplitudeVisualizer {
  public:
    AmplitudeMeterVisualizer(
        LightStrip& strip,
        float note_increase,
        float decrease_rate,
        float max_value);

    virtual void renderValue(float value);
};

#endif

