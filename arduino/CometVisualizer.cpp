#include "CometVisualizer.h"
#include "ColorCycleParticle.h"
#include "SimpleMovingParticle.h"

static bool initialized = false;
static const int num_comet_tail_colors = 64;
static Color comet_tail_colors[num_comet_tail_colors];

class CometParticle : public SimpleMovingParticle {
  public:
    CometParticle(int start_pos, int speed, int min_pos, int max_pos);
    virtual bool age(ParticleVisualizer* pv, unsigned int millis);
};

CometParticle::CometParticle(
    int start_pos, int speed, int min_pos, int max_pos)
        : SimpleMovingParticle(
              start_pos, speed, min_pos, max_pos, Colors::WHITE) {
}

bool CometParticle::age(ParticleVisualizer* pv, unsigned int millis) {
  // TODO this should actually add them uniformly over time/distance, not
  // just one per cycle
  pv->addParticle(
      new ColorCycleParticle(pos, comet_tail_colors, num_comet_tail_colors, 1));
  return SimpleMovingParticle::age(pv, millis);
}

CometVisualizer::CometVisualizer(LPD8806* strip, int max_particles)
    : ParticleVisualizer(strip, max_particles) {
  if (!initialized) {
    for (int i = 0; i < num_comet_tail_colors; ++i) {
      float v = ((float)i / (float)num_comet_tail_colors);
      comet_tail_colors[i] = Colors::hsv(270.0, pow(v, 2), pow(1.0 - v, 0.5));
    }
    initialized = true;
  }
}

void CometVisualizer::onKeyDown(int key) {
  addParticle(new CometParticle(0, 10000, 0, strip->numPixels() - 1));
}
