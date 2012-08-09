#ifndef __INCLUDED_PARTICLE_AGE_HELPER_H
#define __INCLUDED_PARTICLE_AGE_HELPER_H

class Particle;
class ParticleVisualizer;

class ParticleAgeHelper {
  friend class ParticleVisualizer;

  public:
    ~ParticleAgeHelper();
    void addParticle(Particle* p);
    void removeParticle();

  private:
    ParticleAgeHelper(ParticleVisualizer* pv, int max_particles);
    int current_particle_index;
    ParticleVisualizer* const pv;
    int* const removed_particle_indexes;
    int num_removed_particles;
};

#endif

