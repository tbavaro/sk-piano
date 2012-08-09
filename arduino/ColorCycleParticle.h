/**
 * ColorCycleParticle
 *
 * Particle cycles through the color array at a set rate.
 * When the end of the color array is reached, the particle dies.
 */

#ifndef __INCLUDED_COLOR_CYCLE_PARTICLE_H
#define __INCLUDED_COLOR_CYCLE_PARTICLE_H

#include "MovingParticle.h"
#include "Colors.h"

class ColorCycleParticle : public MovingParticle {
  public:
    ColorCycleParticle(
        int pos,
        Color* colors, int num_colors, int color_rate);
    ColorCycleParticle(
        int start_pos, int speed, int min_pos, int max_pos,
        Color* colors, int num_colors, int color_rate);

    virtual bool age(ParticleVisualizer* pv, unsigned int millis);
    virtual void render(LPD8806* strip);

  private:
    const Color* const colors;
    const int num_colors_x1000;
    const int color_rate;

    // current color index x 1000
    unsigned int index_x1000;
};

#endif
