#include "DebugVisualizer.h"
#include "Util.h"

void DebugVisualizer::onKeyDown(Key key) {
  Util::log("down %d", key);
}

void DebugVisualizer::onKeyUp(Key key) {
  Util::log("up %d", key);
}
