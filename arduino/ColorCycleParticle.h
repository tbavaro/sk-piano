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
        Pixel pos,
        const Color* colors, 
        uint16_t num_colors,
        float color_rate);
    ColorCycleParticle(
        Pixel pos,
        float speed,
        Pixel min_pos,
        Pixel max_pos,
        const Color* colors,
        uint16_t num_colors,
        float color_rate);

    virtual bool age(ParticleVisualizer* pv, TimeInterval millis);
    virtual void render(LPD8806* strip);

  private:
    const Color* const colors;
    const uint32_t num_colors_mult;
    const uint32_t color_rate_mult_per_ms;

    // current color index x 1000
    uint32_t index_mult;
};

#endif
