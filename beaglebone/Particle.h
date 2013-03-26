class Particle;

#ifndef __INCLUDED_PARTICLE_H
#define __INCLUDED_PARTICLE_H

#include "LightStrip.h"
#include "SKTypes.h"

class ParticleVisualizer;

class Particle {
  public:
    /**
     * Age the particle by 'millis' milliseconds.
     *
     * @return TRUE iff the particle is still alive
     */
    virtual bool age(ParticleVisualizer* pv, TimeInterval millis)=0;

    virtual void render(LightStrip& strip)=0;

    void kill();

    bool isDead();

  private:
    bool is_dead;
};

inline void Particle::kill() {
  is_dead = true;
}

inline bool Particle::isDead() {
  return is_dead;
}

#endif

