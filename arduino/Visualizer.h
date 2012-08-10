#ifndef __INCLUDED_VISUALIZER_H
#define __INCLUDED_VISUALIZER_H

#include "LPD8806.h"
#include "PianoDelegate.h"

class Visualizer : public PianoDelegate {
  public:
    Visualizer(LPD8806* strip);

    virtual void reset();

  protected:
    LPD8806* const strip;
};

#endif
