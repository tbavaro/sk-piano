#include "PianoLocations.h"

LogicalLightStrip* PianoLocations::directlyAbovePixels(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::SECOND_ROW_LEFTMOST_ABOVE_KEYS,
      PianoLocations::SECOND_ROW_RIGHTMOST_ABOVE_KEYS);
}

