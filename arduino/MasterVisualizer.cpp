#include "MasterVisualizer.h"

static const int MAX_VISUALIZERS = 64;

MasterVisualizer::MasterVisualizer(LPD8806* strip) : Visualizer(strip) {
  // start out with a dummy visualizer, but we'll kill this when we add the
  // first "real" visualizer
  current_viz = new Visualizer(strip);
  current_viz_index = -1;

  // note: arduino c++ doesn't seem to support "new Visualizer*[64]" syntax..?
  visualizers = (Visualizer**)malloc(MAX_VISUALIZERS * sizeof(Visualizer*));

  num_visualizers = 0;
}

MasterVisualizer::~MasterVisualizer() {
  // destroy our dummy visualizer
  if (num_visualizers == 0) {
    delete current_viz;
  }

  free(visualizers);
}

void MasterVisualizer::addVisualizer(Visualizer* viz) {
  // don't add too many visualizers
  if (num_visualizers >= MAX_VISUALIZERS) {
    return;
  }

  bool is_first = (num_visualizers == 0);

  visualizers[num_visualizers++] = viz;
  
  // if this is the first visualizer to be added, destroy our dummy visualizer
  // and enable the visualizer
  if (is_first) {
    delete current_viz;
    this->nextVisualizer();
  }
}

void MasterVisualizer::nextVisualizer() {
  // don't do anything if we don't have any visualizers
  if (num_visualizers == 0) {
    return;
  }

  // increment, wrapping around
  current_viz_index = (current_viz_index + 1) % num_visualizers;
  current_viz = visualizers[current_viz_index];
  
  // reset the visualizer
  current_viz->reset();
}

void MasterVisualizer::reset() {
  // if we have a visualizer, reset to the first one
  if (num_visualizers > 0) {
    current_viz_index = 0;
    current_viz = visualizers[0];
  }

  // do the visualizer base class reset (reset the strip to black)
  Visualizer::reset();

  // reset the current visualizer
  current_viz->reset();
}

void MasterVisualizer::onKeyDown(int key) {
  // if this is the first key (A0), go to the next visualizer
  if (key == 0) {
    this->nextVisualizer();
  }

  current_viz->onKeyDown(key);
}

void MasterVisualizer::onKeyUp(int key) {
  current_viz->onKeyUp(key);
}

void MasterVisualizer::onPassFinished(bool something_changed) {
  current_viz->onPassFinished(something_changed);
}

