#ifndef __INCLUDED_COMPOSITE_VISUALIZER_H
#define __INCLUDED_COMPOSITE_VISUALIZER_H

#include "Visualizer.h"

#include <vector>

class CompositeVisualizer : public Visualizer {
  public:
    CompositeVisualizer();
    ~CompositeVisualizer();

    /**
     * Adds the specified visualizer.  CompositeVisualizer will take care of
     * destroying it when it is destroyed.
     **/
    void addVisualizer(Visualizer* vis);

    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
    virtual void reset();

  private:
    std::vector<Visualizer*> visualizers;
};

#endif

