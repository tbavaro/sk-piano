#include "MasterVisualizer.h"

static const uint8_t MAX_VISUALIZERS = 64;

MasterVisualizer::MasterVisualizer(LPD8806* strip)
    : Visualizer(strip), 
      visualizer_factories((VisualizerFactory*)malloc(MAX_VISUALIZERS * sizeof(VisualizerFactory))) {
  // start out with a dummy visualizer, but we'll kill this when we add the
  // first "real" visualizer
  current_viz = new Visualizer(strip);
  current_viz_index = -1;
  num_visualizers = 0;
}

MasterVisualizer::~MasterVisualizer() {
  // destroy our dummy visualizer
  if (num_visualizers == 0) {
    delete current_viz;
  }

  free(visualizer_factories);
}

void MasterVisualizer::addVisualizer(VisualizerFactory viz_factory) {
  // don't add too many visualizers
  if (num_visualizers >= MAX_VISUALIZERS) {
    return;
  }

  bool is_first = (num_visualizers == 0);

  visualizer_factories[num_visualizers++] = viz_factory;
  
  // if this is the first visualizer to be added, destroy our dummy visualizer
  // and enable the visualizer
  if (is_first) {
    this->nextVisualizer();
  }
}

void MasterVisualizer::nextVisualizer() {
  // don't do anything if we don't have any visualizers
  if (num_visualizers == 0) {
    return;
  }

  // destroy the current visualizer
  delete current_viz;

  // increment, wrapping around
  current_viz_index = (current_viz_index + 1) % num_visualizers;
  current_viz = visualizer_factories[current_viz_index]();
  
  // reset the visualizer
  current_viz->reset();
}

void MasterVisualizer::reset() {
  // if we have a visualizer, reset to the first one
  delete current_viz;
  if (num_visualizers > 0) {
    current_viz_index = 0;
    current_viz = visualizer_factories[current_viz_index]();
  } else {
    current_viz_index = -1;
    current_viz = new Visualizer(strip);
  }

  // do the visualizer base class reset (reset the strip to black)
  Visualizer::reset();

  // reset the current visualizer
  current_viz->reset();
}

void MasterVisualizer::onKeyDown(Key key) {
  // if this is the first key (A0), go to the next visualizer
  if (key == 0) {
    this->nextVisualizer();
  }

  current_viz->onKeyDown(key);
}

void MasterVisualizer::onKeyUp(Key key) {
  current_viz->onKeyUp(key);
}

void MasterVisualizer::onPassFinished(bool something_changed) {
  current_viz->onPassFinished(something_changed);
}

