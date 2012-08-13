#ifndef __INCLUDED_VISUALIZER_H
#define __INCLUDED_VISUALIZER_H

#include "LightStrip.h"
#include "PianoDelegate.h"

class Visualizer : public PianoDelegate {
  public:
    Visualizer(LightStrip& strip);

    virtual void reset();

  protected:
    LightStrip& strip;
};

#endif
