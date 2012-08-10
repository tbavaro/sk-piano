#ifndef __INCLUDED_COMET_VISUALIZER_H
#define __INCLUDED_COMET_VISUALIZER_H

#include "ParticleVisualizer.h"
#include "SKTypes.h"

class CometVisualizer : public ParticleVisualizer {
  public:
    CometVisualizer(LPD8806* strip, uint16_t max_particles);
    virtual void onKeyDown(Key key);
};

#endif
