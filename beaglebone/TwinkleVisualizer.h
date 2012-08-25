#ifndef __INCLUDED_TWINKLE_VISUALIZER_H
#define __INCLUDED_TWINKLE_VISUALIZER_H

#include "LightStrip.h"
#include "AbstractAmplitudeVisualizer.h"

class TwinkleVisualizer : public AbstractAmplitudeVisualizer {
  public:
    TwinkleVisualizer(LightStrip& strip);
    ~TwinkleVisualizer();

    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void renderValue(float value);

  private:
    float* pixel_values;
    int pixelForKey(Key key);
};

#endif

