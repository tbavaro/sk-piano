#include "DebugVisualizer.h"
#include "Util.h"

DebugVisualizer::DebugVisualizer() : Visualizer() {
}

void DebugVisualizer::onKeyDown(Key key) {
  Util::log("down %d", key);
}

void DebugVisualizer::onKeyUp(Key key) {
  Util::log("up %d", key);
}

void DebugVisualizer::onPassFinished(bool something_changed) {
}
