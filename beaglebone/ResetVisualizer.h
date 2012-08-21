#ifndef __INCLUDED_RESET_VISUALIZER_H
#define __INCLUDED_RESET_VISUALIZER_H

#include "LightStripVisualizer.h"

/**
 * A simple visualizer that just blacks out the strip on every frame.  Use
 * CompositeVisualizer along with this to reset the strip before other
 * visualizations that don't reset the pixels (e.g. because they are additive)
 */

class ResetVisualizer : public LightStripVisualizer {
  public:
    ResetVisualizer(LightStrip& strip);
    virtual void onPassFinished(bool something_changed);
};

#endif
