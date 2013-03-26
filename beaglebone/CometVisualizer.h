#ifndef __INCLUDED_COMET_VISUALIZER_H
#define __INCLUDED_COMET_VISUALIZER_H

#include "ParticleVisualizer.h"
#include "SKTypes.h"

class CometVisualizer : public ParticleVisualizer {
  public:
    CometVisualizer(LightStrip& strip, uint16_t max_particles);
    ~CometVisualizer();
    virtual void onKeyDown(Key key);
};

#endif
