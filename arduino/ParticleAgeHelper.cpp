#include "ParticleAgeHelper.h"

ParticleAgeHelper::ParticleAgeHelper(
    ParticleVisualizer* _pv, int max_particles) {
  current_particle_index = 0;
  pv = _pv;
  removed_particle_indexes = new int[max_particles];
  num_removed_particles = 0;
}

ParticleAgeHelper::~ParticleAgeHelper() {
  delete[] removed_particle_indexes;
}

