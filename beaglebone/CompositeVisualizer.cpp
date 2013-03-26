#include "CompositeVisualizer.h"

using namespace std;

CompositeVisualizer::CompositeVisualizer() : Visualizer() {
}

CompositeVisualizer::~CompositeVisualizer() {
  for (vector<Visualizer*>::iterator i = visualizers.begin();
       i != visualizers.end();
       ++i) {
    delete *i;
  }
}

void CompositeVisualizer::addVisualizer(Visualizer* vis) {
  visualizers.push_back(vis);
}

void CompositeVisualizer::onKeyDown(Key key) {
  for (vector<Visualizer*>::iterator i = visualizers.begin();
       i != visualizers.end();
       ++i) {
    (*i)->onKeyDown(key);
  }
}

void CompositeVisualizer::onKeyUp(Key key) {
  for (vector<Visualizer*>::iterator i = visualizers.begin();
       i != visualizers.end();
       ++i) {
    (*i)->onKeyUp(key);
  }
}

void CompositeVisualizer::onPassFinished(bool something_changed) {
  for (vector<Visualizer*>::iterator i = visualizers.begin();
       i != visualizers.end();
       ++i) {
    (*i)->onPassFinished(something_changed);
  }
}

void CompositeVisualizer::reset() {
  for (vector<Visualizer*>::iterator i = visualizers.begin();
       i != visualizers.end();
       ++i) {
    (*i)->reset();
  }
}

