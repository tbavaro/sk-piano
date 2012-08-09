#ifndef __INCLUDED_PARTICLE_AGE_HELPER_H
#define __INCLUDED_PARTICLE_AGE_HELPER_H

#include "ParticleVisualizer.h"

class ParticleAgeHelper {
  friend class ParticleVisualizer;

  public:
    ~ParticleAgeHelper();
    void addParticle(Particle* p);
    void removeParticle();

  private:
    ParticleAgeHelper(ParticleVisualizer* pv, int max_particles);
    int current_particle_index;
    ParticleVisualizer* pv;
    int* removed_particle_indexes;
    int num_removed_particles;
};

inline void ParticleAgeHelper::addParticle(Particle* p) {
  pv->addParticle(p);
}

inline void ParticleAgeHelper::removeParticle() {
  removed_particle_indexes[num_removed_particles++] = current_particle_index;
}

#endif

