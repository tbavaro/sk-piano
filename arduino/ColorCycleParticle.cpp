#include "ColorCycleParticle.h"

ColorCycleParticle::ColorCycleParticle(
        int pos, Color* colors, int num_colors, int color_rate)
        : MovingParticle(pos, 0, pos, pos),
          colors(colors),
          num_colors_x1000(num_colors * 1000), 
          color_rate(color_rate),
          index_x1000(0) {
}

ColorCycleParticle::ColorCycleParticle(
    int start_pos, int speed, int min_pos, int max_pos,
    Color* colors, int num_colors, int color_rate)
        : MovingParticle(start_pos, speed, min_pos, max_pos),
          colors(colors), 
          num_colors_x1000(num_colors * 1000), 
          color_rate(color_rate),
          index_x1000(0) {
}

bool ColorCycleParticle::age(ParticleVisualizer* pv, unsigned int millis) {
  bool alive = MovingParticle::age(pv, millis);

  if (alive) {
    index_x1000 += color_rate * millis;
    if (index_x1000 >= num_colors_x1000) {
      alive = false;
    }
  }

  return alive;
}

void ColorCycleParticle::render(LPD8806* strip) {
  strip->addPixelColor(pos, colors[index_x1000 / 1000]);
}

