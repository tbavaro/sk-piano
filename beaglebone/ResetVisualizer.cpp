#include "ResetVisualizer.h"

ResetVisualizer::ResetVisualizer(LightStrip& strip) 
    : LightStripVisualizer(strip) {
}

void ResetVisualizer::onPassFinished(bool something_changed) {
  LightStripVisualizer::reset();
}

