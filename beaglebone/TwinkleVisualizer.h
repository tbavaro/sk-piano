#ifndef __INCLUDED_TWINKLE_VISUALIZER_H
#define __INCLUDED_TWINKLE_VISUALIZER_H

#include "LightStrip.h"
#include "AbstractAmplitudeVisualizer.h"

class TwinkleVisualizer : public AbstractAmplitudeVisualizer {
  public:
    TwinkleVisualizer(LightStrip& strip, bool highlight_keys);
    ~TwinkleVisualizer();

    virtual void reset();
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void renderValue(float value);

  private:
    float* pixel_values;
    float* pixel_saturations;
    bool* keys;
    int pixelForKey(Key key);
    float sparkle_accum;
    bool highlight_keys;
};

#endif

