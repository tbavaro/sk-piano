#ifndef __INCLUDED_MASTER_VISUALIZER_H
#define __INCLUDED_MASTER_VISUALIZER_H

#include "Visualizer.h"

class MasterVisualizer : public Visualizer {
  public:
    MasterVisualizer(LPD8806* strip);
    ~MasterVisualizer();

    void addVisualizer(Visualizer* viz);
    void nextVisualizer();

    virtual void reset();
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);

  private:
    Visualizer* current_viz;
    uint8_t current_viz_index;
    Visualizer** const visualizers;
    uint8_t num_visualizers;
};

#endif
