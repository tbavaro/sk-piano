#ifndef __INCLUDED_SIMPLE_MOVING_PARTICLE_H
#define __INCLUDED_SIMPLE_MOVING_PARTICLE_H

#include "MovingParticle.h"
#include "Colors.h"

class SimpleMovingParticle : public MovingParticle {
  public:
    SimpleMovingParticle(
        int start_pos, int speed, int min_pos, int max_pos, Color color);

    virtual void render(LPD8806* strip);

  private:
    const Color color;
};

#endif

