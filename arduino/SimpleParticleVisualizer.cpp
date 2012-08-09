#include "SimpleParticleVisualizer.h"
#include "SimpleMovingParticle.h"
#include "Colors.h"

SimpleParticleVisualizer::SimpleParticleVisualizer(
    LPD8806* strip, int max_particles) 
        : ParticleVisualizer(strip, max_particles) {
}

void SimpleParticleVisualizer::onKeyDown(int key) {
  Color color = Colors::rainbow(key * 4);
  addParticle(
    new SimpleMovingParticle(0, 10000, 0, strip->numPixels() - 1, color));
}
