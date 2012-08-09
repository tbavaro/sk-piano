#include "MovingParticle.h"

MovingParticle::MovingParticle(
    int start_pos, int speed, int min_pos, int max_pos)
      : pos(start_pos), speed(speed), min_pos(min_pos), max_pos(max_pos) {
}

bool MovingParticle::age(ParticleVisualizer* pv, unsigned int millis) {
  pos = pos + speed * millis;

  // if the particle has moved out of bounds, kill it
  return (pos >= min_pos && pos <= max_pos);
}
