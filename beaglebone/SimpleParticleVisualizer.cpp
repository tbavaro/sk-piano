#include "SimpleParticleVisualizer.h"
#include "SimpleMovingParticle.h"
#include "Colors.h"

SimpleParticleVisualizer::SimpleParticleVisualizer(
    LightStrip& strip, int max_particles) 
        : ParticleVisualizer(strip, max_particles) {
}

void SimpleParticleVisualizer::onKeyDown(Key key) {
  Color color = Colors::rainbow(key * 4);
  addParticle(
    new SimpleMovingParticle((uint32_t)key * 1000, 10, 0, ((uint32_t)(strip.numPixels() - 1)) * 1000, color));
  addParticle(
    new SimpleMovingParticle((uint32_t)key * 1000, -10, 0, ((uint32_t)(strip.numPixels() - 1)) * 1000, color));
}
