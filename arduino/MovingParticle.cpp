#include "MovingParticle.h"

const int32_t MovingParticle::PIXEL_MULTIPLIER = 65536;

MovingParticle::MovingParticle(
    Pixel start_pos, float speed, Pixel min_pos, Pixel max_pos)
      : pos_mult(start_pos * PIXEL_MULTIPLIER),
        speed_mult_per_ms(speed * PIXEL_MULTIPLIER / 1000),
        min_pos_mult(min_pos * PIXEL_MULTIPLIER),
        max_pos_mult(max_pos * PIXEL_MULTIPLIER) {
};

bool MovingParticle::age(ParticleVisualizer* pv, TimeInterval millis) {
  pos_mult += speed_mult_per_ms * ((int32_t)millis);

  // if the particle has moved out of bounds, kill it
  return (pos_mult >= min_pos_mult && pos_mult <= max_pos_mult);
}
