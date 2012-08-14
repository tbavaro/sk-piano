#ifndef __INCLUDED_DEBUG_VISUALIZER_H
#define __INCLUDED_DEBUG_VISUALIZER_H

#include "Visualizer.h"

class DebugVisualizer : public Visualizer {
  public:
    DebugVisualizer(LightStrip& strip);
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
};

#endif
