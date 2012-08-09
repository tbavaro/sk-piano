class ParticleVisualizer;

#ifndef __INCLUDED_PARTICLE_VISUALIZER_H
#define __INCLUDED_PARTICLE_VISUALIZER_H

#include "Visualizer.h"
#include "Particle.h"

class ParticleVisualizer : public Visualizer {
  public:
    ParticleVisualizer(LPD8806* strip, int max_particles);
    ~ParticleVisualizer();

    virtual void reset();
    virtual void onPassFinished(bool something_changed);

    void addParticle(Particle* p);

  private:
    void removeAllParticles();

    Particle** const particles;
    int num_particles;
    int num_particles_after_last_frame;
    const int max_particles;

    unsigned long prev_frame_time;
    
    int* const removed_particle_indexes;
};

#endif

