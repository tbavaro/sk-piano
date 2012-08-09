#ifndef __INCLUDED_COMET_VISUALIZER_H
#define __INCLUDED_COMET_VISUALIZER_H

#include "ParticleVisualizer.h"

class CometVisualizer : public ParticleVisualizer {
  public:
    CometVisualizer(LPD8806* strip, int max_particles);
    virtual void onKeyDown(int key);
};

#endif
