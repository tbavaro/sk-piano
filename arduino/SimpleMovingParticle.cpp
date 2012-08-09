#include "SimpleMovingParticle.h"

SimpleMovingParticle::SimpleMovingParticle(
    int start_pos, int speed, int min_pos, int max_pos, Color color)
        : MovingParticle(start_pos, speed, min_pos, max_pos), color(color) {
}

void SimpleMovingParticle::render(LPD8806* strip) {
  int strip_pos = pos / 1000;
  strip->addPixelColor(strip_pos, color);
}
