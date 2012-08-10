#ifndef __INCLUDED_MASTER_VISUALIZER_H
#define __INCLUDED_MASTER_VISUALIZER_H

#include "Visualizer.h"

typedef Visualizer*(*VisualizerFactory)(void);

class MasterVisualizer : public Visualizer {
  public:
    MasterVisualizer(LPD8806* strip);
    ~MasterVisualizer();

    void addVisualizer(VisualizerFactory viz_factory);
    void nextVisualizer();

    virtual void reset();
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);

  private:
    Visualizer* current_viz;
    int8_t current_viz_index;
    VisualizerFactory* const visualizer_factories;
    uint8_t num_visualizers;
};

#endif
