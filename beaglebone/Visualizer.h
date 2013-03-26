#ifndef __INCLUDED_VISUALIZER_H
#define __INCLUDED_VISUALIZER_H

#include "PianoDelegate.h"

class Visualizer : public PianoDelegate {
  public:
    virtual void reset();
};

#endif
