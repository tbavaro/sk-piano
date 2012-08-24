#ifndef __INCLUDED_RAINDROPS_VISUALIZER_H
#define __INCLUDED_RAINDROPS_VISUALIZER_H

#include "LightStrip.h"
#include "AbstractAmplitudeVisualizer.h"

class RaindropsVisualizer : public AbstractAmplitudeVisualizer {
  public:
    RaindropsVisualizer(LightStrip& strip);
    ~RaindropsVisualizer();

    virtual void renderValue(float value);

  private:
    Color* pixels;
    Color* old_pixels;
};

#endif
