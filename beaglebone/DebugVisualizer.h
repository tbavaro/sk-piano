#ifndef __INCLUDED_DEBUG_VISUALIZER_H
#define __INCLUDED_DEBUG_VISUALIZER_H

#include "Visualizer.h"

class DebugVisualizer : public Visualizer {
  public:
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
};

#endif
