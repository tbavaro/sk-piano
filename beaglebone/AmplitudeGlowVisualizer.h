#ifndef __INCLUDED_AMPLITUDE_GLOW_VISUALIZER_H
#define __INCLUDED_AMPLITUDE_GLOW_VISUALIZER_H

#include "AbstractAmplitudeVisualizer.h"

class AmplitudeGlowVisualizer : public AbstractAmplitudeVisualizer {
  public:
    AmplitudeGlowVisualizer(
        LightStrip& strip,
        float note_increase,
        float decrease_rate,
        float max_value);

    virtual void renderValue(float value);
};

#endif

