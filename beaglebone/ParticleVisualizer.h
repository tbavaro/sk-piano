class ParticleVisualizer;

#ifndef __INCLUDED_PARTICLE_VISUALIZER_H
#define __INCLUDED_PARTICLE_VISUALIZER_H

#include "LightStripVisualizer.h"
#include "Particle.h"

class ParticleVisualizer : public LightStripVisualizer {
  public:
    ParticleVisualizer(LightStrip& strip, uint16_t max_particles);
    ~ParticleVisualizer();

    virtual void reset();
    virtual void onPassFinished(bool something_changed);

    void addParticle(Particle* p);

  private:
    void removeAllParticles();

    Particle** const particles;
    uint16_t num_particles;
    uint16_t num_particles_after_last_frame;
    const uint16_t max_particles;

    uint32_t prev_frame_time;
    
    uint16_t* const removed_particle_indexes;
};

#endif

