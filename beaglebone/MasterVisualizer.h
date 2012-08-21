#ifndef __INCLUDED_MASTER_VISUALIZER_H
#define __INCLUDED_MASTER_VISUALIZER_H

#include "Visualizer.h"
#include "LightStrip.h"

class MasterVisualizer : public Visualizer {
  public:
    MasterVisualizer(LightStrip& strip);
    ~MasterVisualizer();

    void addVisualizer(Visualizer* visualizer);
    void nextVisualizer();

    virtual void reset();
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);

  private:
    LightStrip& strip;
    Visualizer* current_viz;
    int8_t current_viz_index;
    Visualizer** const visualizers;
    uint8_t num_visualizers;
};

#endif
