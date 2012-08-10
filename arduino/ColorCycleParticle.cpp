#include "ColorCycleParticle.h"

ColorCycleParticle::ColorCycleParticle(
        Pixel pos, const Color* colors, uint16_t num_colors, float color_rate)
        : MovingParticle(pos, 0, pos, pos),
          colors(colors),
          num_colors_mult(((uint32_t)num_colors) * INDEX_MULTIPLIER),
          color_rate_mult_per_ms(color_rate * INDEX_MULTIPLIER / 1000),
          index_mult(0) {
}

ColorCycleParticle::ColorCycleParticle(
    Pixel start_pos, float speed, Pixel min_pos, Pixel max_pos,
    const Color* colors, uint16_t num_colors, float color_rate)
        : MovingParticle(start_pos, speed, min_pos, max_pos),
          colors(colors),
          num_colors_mult(((uint32_t)num_colors) * INDEX_MULTIPLIER),
          color_rate_mult_per_ms(color_rate * INDEX_MULTIPLIER / 1000),
          index_mult(0) {
}

bool ColorCycleParticle::age(ParticleVisualizer* pv, TimeInterval millis) {
  bool alive = MovingParticle::age(pv, millis);

  if (alive) {
    index_mult += color_rate_mult_per_ms * ((uint32_t)millis);
    if (index_mult >= num_colors_mult) {
      alive = false;
    }
  }

  return alive;
}

void ColorCycleParticle::render(LPD8806* strip) {
  strip->addPixelColor(pos(), colors[index()]);
}

