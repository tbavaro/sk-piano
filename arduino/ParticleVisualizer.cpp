#include "ParticleAgeHelper.h"
#include "ParticleVisualizer.h"

static unsigned long TIME_NONE = 0;

ParticleVisualizer::ParticleVisualizer(LPD8806* strip, int max_particles) 
    : Visualizer(strip), 
      max_particles(max_particles),
      particles((Particle**)malloc(sizeof(Particle) * max_particles)),
      age_helper(new ParticleAgeHelper(this, max_particles)) {
  num_particles = 0;
  num_particles_after_last_frame = 0;
  prev_frame_time = TIME_NONE;
}

ParticleVisualizer::~ParticleVisualizer() {
  // clean up any remaining particles
  this->removeAllParticles();
  
  // clean up array
  free(particles);

  delete age_helper;
}

void ParticleVisualizer::reset() {
  prev_frame_time = TIME_NONE;
  this->removeAllParticles();
  Visualizer::reset();
}

void ParticleVisualizer::onPassFinished(bool something_changed) {
  // advance the clock and see how long it's been since the last frame
  unsigned long now = millis();
  unsigned int frame_duration;
  if (prev_frame_time == TIME_NONE) {
    frame_duration = 0;
  } else {
    frame_duration = now - prev_frame_time;
  }
  prev_frame_time = now;

  // advance particles; don't advance new particles added this round
  for (int i = 0; i < num_particles_after_last_frame; ++i) {
    age_helper->current_particle_index = i;
    Particle* particle = particles[i];
    particle->age(age_helper, frame_duration);
  }

  // remove dead particles
  for (int i = age_helper->num_removed_particles - 1; i >= 0; --i) {
    int index = age_helper->removed_particle_indexes[i];
    
    // delete the dead particle
    delete particles[index];

    // if this wasn't the last particle, swap that one into this place
    int last_index = num_particles - 1;
    if (index < last_index) {
      particles[index] = particles[last_index];
    }

    --num_particles;
  }
  age_helper->num_removed_particles = 0;
  num_particles_after_last_frame = num_particles;

  // reset strip to black
  strip->reset();

  // render particles
  for (int i = 0; i < num_particles; ++i) {
    particles[i]->render(strip);
  }

  // update strip
  strip->show();
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
