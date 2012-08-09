#ifndef __INCLUDED_PARTICLE_VISUALIZER_H
#define __INCLUDED_PARTICLE_VISUALIZER_H

#include "Visualizer.h"
#include "Particle.h"

class ParticleAgeHelper;

class ParticleVisualizer : public Visualizer {
  public:
    ParticleVisualizer(LPD8806* strip, int max_particles);
    ~ParticleVisualizer();

    virtual void reset();
    virtual void onPassFinished(bool something_changed);

    void addParticle(Particle* p);
    void removeParticle();

  private:
    void removeAllParticles();

    ParticleAgeHelper* age_helper;

    Particle** particles;
    int num_particles;
    int max_particles;

    unsigned long prev_frame_time;
};

#endif

