#include "ParticleAgeHelper.h"
#include "ParticleVisualizer.h"

ParticleAgeHelper::ParticleAgeHelper(
    ParticleVisualizer* pv, int max_particles) 
        : pv(pv),
          removed_particle_indexes((int*)malloc(sizeof(int) * max_particles)) {
  current_particle_index = 0;
  num_removed_particles = 0;
}

ParticleAgeHelper::~ParticleAgeHelper() {
  free(removed_particle_indexes);
}

void ParticleAgeHelper::addParticle(Particle* p) {
  pv->addParticle(p);
}

void ParticleAgeHelper::removeParticle() {
  removed_particle_indexes[num_removed_particles++] = current_particle_index;
}

