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
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual void onPassFinished(bool something_changed);

  private:
    Visualizer* current_viz;
    int current_viz_index;
    Visualizer** visualizers;
    int num_visualizers;
};

#endif
