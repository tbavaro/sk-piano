#include "CometVisualizer.h"
#include "ColorCycleParticle.h"
#include "SimpleMovingParticle.h"

#include <math.h>

static const uint16_t NUM_COMET_TAIL_COLORS = 16;
static Color* comet_tail_colors;

class CometParticle : public SimpleMovingParticle {
  public:
    CometParticle(Pixel start_pos, float speed, Pixel min_pos, Pixel max_pos);
    virtual bool age(ParticleVisualizer* pv, TimeInterval millis);
};

CometParticle::CometParticle(
    Pixel start_pos, float speed, Pixel min_pos, Pixel max_pos)
        : SimpleMovingParticle(
              start_pos, speed, min_pos, max_pos, Colors::WHITE) {
}

bool CometParticle::age(ParticleVisualizer* pv, TimeInterval millis) {
  // TODO this should actually add them uniformly over time/distance, not
  // just one per cycle
  pv->addParticle(
      new ColorCycleParticle(
        pos(), 
        comet_tail_colors, 
        NUM_COMET_TAIL_COLORS, 
        1));
  return SimpleMovingParticle::age(pv, millis);
}

CometVisualizer::CometVisualizer(LightStrip& strip, uint16_t max_particles)
    : ParticleVisualizer(strip, max_particles) {
  comet_tail_colors = new Color[NUM_COMET_TAIL_COLORS];
  for (int i = 0; i < NUM_COMET_TAIL_COLORS; ++i) {
    float v = ((float)i / (float)NUM_COMET_TAIL_COLORS);
    comet_tail_colors[i] = Colors::hsv(270.0, pow(v, 2), pow(1.0 - v, 0.5));
  }
}

CometVisualizer::~CometVisualizer() {
  delete[] comet_tail_colors;
}

void CometVisualizer::onKeyDown(Key key) {
  addParticle(new CometParticle(0, 1.0, 0, strip.numPixels() - 1));
}
