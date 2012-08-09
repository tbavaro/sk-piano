#ifndef __INCLUDED_PARTICLE_H
#define __INCLUDED_PARTICLE_H

#include "LPD8806.h"

class ParticleAgeHelper;

class Particle {
  public:
    virtual void age(ParticleAgeHelper* helper, unsigned long millis)=0;
    virtual void render(LPD8806* strip)=0;
};

#endif

