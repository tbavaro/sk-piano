#include "MovingParticle.h"

MovingParticle::MovingParticle(
    int _start_pos, int _speed, int _min_pos, int _max_pos)
      : min_pos(_min_pos), max_pos(_max_pos) {
  pos = _start_pos;
  speed = _speed;
}

void MovingParticle::age(ParticleAgeHelper* helper, unsigned int millis) {
  pos = pos + speed * millis;

  // if the particle has moved out of bounds, kill it
  if (pos < min_pos || pos > max_pos) {
    helper->removeParticle();
  }
}
