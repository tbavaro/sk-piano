#ifndef __DEFINED_MOVING_PARTICLE_H
#define __DEFINED_MOVING_PARTICLE_H

#include "Particle.h"

class MovingParticle : public Particle {
  public:
    /**
     * @param start_pos starting position (location x 1000)
     * @param speed per ms
     */
    MovingParticle(int start_pos, int speed, int min_pos, int max_pos);

    virtual void age(ParticleAgeHelper* helper, unsigned int millis);

  protected:
    int pos;
    int speed;
    const int min_pos;
    const int max_pos;
};

#endif
