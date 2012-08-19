#ifndef __INCLUDED_LIGHT_STRIP_VISUALIZER_H
#define __INCLUDED_LIGHT_STRIP_VISUALIZER_H

#include "LightStrip.h"
#include "Visualizer.h"

class LightStripVisualizer : public Visualizer {
  public:
    LightStripVisualizer(LightStrip& strip);
    virtual void reset();

  protected:
    LightStrip& strip;
};

#endif
