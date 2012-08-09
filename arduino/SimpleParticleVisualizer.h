#ifndef __INCLUDED_SIMPLE_PARTICLE_VISUALIZER_H
#define __INCLUDED_SIMPLE_PARTICLE_VISUALIZER_H

#include "ParticleVisualizer.h"

class SimpleParticleVisualizer : public ParticleVisualizer {
  public:
    SimpleParticleVisualizer(LPD8806* strip, int max_particles);

    virtual void onKeyDown(int key);
};

#endif

