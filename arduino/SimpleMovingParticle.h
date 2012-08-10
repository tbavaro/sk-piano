#ifndef __INCLUDED_SIMPLE_MOVING_PARTICLE_H
#define __INCLUDED_SIMPLE_MOVING_PARTICLE_H

#include "MovingParticle.h"
#include "Colors.h"

class SimpleMovingParticle : public MovingParticle {
  public:
    SimpleMovingParticle(
        uint32_t start_pos, uint32_t speed, uint32_t min_pos, uint32_t max_pos, Color color);

    virtual void render(LPD8806* strip);

  private:
    const Color color;
};

#endif

