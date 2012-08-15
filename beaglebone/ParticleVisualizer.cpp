#include "ParticleVisualizer.h"
#include "Util.h"

static const uint32_t TIME_NONE = 0;

ParticleVisualizer::ParticleVisualizer(LightStrip& strip, uint16_t max_particles) 
    : LightStripVisualizer(strip), 
      max_particles(max_particles),
      particles(new Particle*[max_particles]),
      removed_particle_indexes(new uint16_t[max_particles]) {
  num_particles = 0;
  num_particles_after_last_frame = 0;
  prev_frame_time = TIME_NONE;
}

ParticleVisualizer::~ParticleVisualizer() {
  // clean up any remaining particles
  this->removeAllParticles();
  
  // clean up arrays
  delete[] particles;
  delete[] removed_particle_indexes;
}

void ParticleVisualizer::reset() {
  prev_frame_time = TIME_NONE;
  this->removeAllParticles();
  Visualizer::reset();
}

void ParticleVisualizer::onPassFinished(bool something_changed) {
  // advance the clock and see how long it's been since the last frame
  uint32_t now = Util::millis();
  TimeInterval frame_duration;
  if (prev_frame_time == TIME_NONE) {
    frame_duration = 0;
  } else {
    frame_duration = now - prev_frame_time;
  }
  prev_frame_time = now;

  uint16_t num_removed_particles = 0;

  // advance particles; don't advance new particles added this round
  for (int i = 0; i < num_particles_after_last_frame; ++i) {
    Particle* particle = particles[i];

    bool alive = particle->age(this, frame_duration);
    if (!alive) {
      removed_particle_indexes[num_removed_particles++] = i;
    }
  }

  // remove dead particles
  for (int i = num_removed_particles - 1; i >= 0; --i) {
    uint16_t index = removed_particle_indexes[i];
    
    // delete the dead particle
    delete particles[index];

    // if this wasn't the last particle, swap that one into this place
    uint16_t last_index = num_particles - 1;
    if (index < last_index) {
      particles[index] = particles[last_index];
    }

    --num_particles;
  }
  num_particles_after_last_frame = num_particles;

  // reset strip to black
  strip.reset();

  // render particles
  for (int i = 0; i < num_particles; ++i) {
    particles[i]->render(strip);
  }
}

void ParticleVisualizer::addParticle(Particle* p) {
  if (num_particles >= max_particles) {
    // can't add any more particles
    return;
  }

  particles[num_particles++] = p;
}

void ParticleVisualizer::removeAllParticles() {
  // clean up any remaining particles
  for (int i = 0; i < num_particles; ++i) {
    delete particles[i];
  }
  num_particles = 0;
}
