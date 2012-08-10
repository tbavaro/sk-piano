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
    static const uint32_t INDEX_MULTIPLIER = 65536;

    uint16_t index() const;

    const Color* const colors;
    const uint32_t num_colors_mult;
    const uint32_t color_rate_mult_per_ms;
    uint32_t index_mult;
};

inline uint16_t ColorCycleParticle::index() const {
  return index_mult / INDEX_MULTIPLIER;
}

#endif
