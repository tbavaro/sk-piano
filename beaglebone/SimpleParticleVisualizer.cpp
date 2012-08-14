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
    new SimpleMovingParticle(0, 50, 0, strip.numPixels() - 1, color));
}
