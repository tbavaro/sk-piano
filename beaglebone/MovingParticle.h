#ifndef __DEFINED_MOVING_PARTICLE_H
#define __DEFINED_MOVING_PARTICLE_H

#include "Particle.h"
#include "SKTypes.h"

class MovingParticle : public Particle {
  public:
    /**
     * @param start_pos starting position (location x 1000)
     * @param speed pos delta per ms
     */
    MovingParticle(Pixel start_pos, float speed, Pixel min_pos, Pixel max_pos);

    virtual bool age(ParticleVisualizer* pv, TimeInterval millis);

  protected:
    static const int32_t PIXEL_MULTIPLIER = 65536;

    Pixel pos() const;

    int32_t pos_mult;
    int32_t speed_mult_per_ms;
    const int32_t min_pos_mult;
    const int32_t max_pos_mult;
};

inline Pixel MovingParticle::pos() const {
  return pos_mult / PIXEL_MULTIPLIER;
}

#endif
