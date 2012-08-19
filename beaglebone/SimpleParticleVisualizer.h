#ifndef __INCLUDED_SIMPLE_PARTICLE_VISUALIZER_H
#define __INCLUDED_SIMPLE_PARTICLE_VISUALIZER_H

#include "ParticleVisualizer.h"

class SimpleParticleVisualizer : public ParticleVisualizer {
  public:
    SimpleParticleVisualizer(LightStrip& strip, int max_particles);

    virtual void onKeyDown(Key key);
};

#endif

