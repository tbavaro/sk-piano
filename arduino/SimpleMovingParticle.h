#ifndef __INCLUDED_SIMPLE_MOVING_PARTICLE_H
#define __INCLUDED_SIMPLE_MOVING_PARTICLE_H

#include "MovingParticle.h"
#include "Colors.h"

class SimpleMovingParticle : public MovingParticle {
  public:
    SimpleMovingParticle(
        Pixel start_pos, float speed, Pixel min_pos, Pixel max_pos, Color color);

    virtual void render(LPD8806* strip);

  private:
    const Color color;
};

#endif

