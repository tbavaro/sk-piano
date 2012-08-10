#ifndef __DEFINED_MOVING_PARTICLE_H
#define __DEFINED_MOVING_PARTICLE_H

#include "Particle.h"

class MovingParticle : public Particle {
  public:
    /**
     * @param start_pos starting position (location x 1000)
     * @param speed pos delta per ms
     */
    MovingParticle(uint32_t start_pos, uint32_t speed, uint32_t min_pos, uint32_t max_pos);

    virtual bool age(ParticleVisualizer* pv, unsigned int millis);

  protected:
    uint32_t pos;
    uint32_t speed;
    const uint32_t min_pos;
    const uint32_t max_pos;
};

#endif
