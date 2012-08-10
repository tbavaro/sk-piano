#include "MovingParticle.h"

MovingParticle::MovingParticle(
    uint32_t start_pos, uint32_t speed, uint32_t min_pos, uint32_t max_pos)
      : pos(start_pos), speed(speed), min_pos(min_pos), max_pos(max_pos) {
}

bool MovingParticle::age(ParticleVisualizer* pv, unsigned int millis) {
  pos = pos + 1000;//speed * millis;

  // if the particle has moved out of bounds, kill it
  return (pos >= min_pos && pos <= max_pos);
}
