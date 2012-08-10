#include "SimpleMovingParticle.h"

SimpleMovingParticle::SimpleMovingParticle(
    uint32_t start_pos, uint32_t speed, uint32_t min_pos, uint32_t max_pos, Color color)
        : MovingParticle(start_pos, speed, min_pos, max_pos), color(color) {
}

void SimpleMovingParticle::render(LPD8806* strip) {
  int strip_pos = pos / 1000;
  strip->addPixelColor(strip_pos, color);
}
