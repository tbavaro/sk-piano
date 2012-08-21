#include "PianoLocations.h"

LogicalLightStrip* PianoLocations::topFrontRow(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::TOP_FRONT_ROW_LEFT,
      PianoLocations::TOP_FRONT_ROW_RIGHT);
}

LogicalLightStrip* PianoLocations::directlyAboveKeys(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::SECOND_ROW_LEFTMOST_ABOVE_KEYS,
      PianoLocations::SECOND_ROW_RIGHTMOST_ABOVE_KEYS);
}

LogicalLightStrip* PianoLocations::upRightLegFront(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::RIGHT_LEG_FRONT_BOTTOM,
      PianoLocations::RIGHT_LEG_FRONT_TOP);
}

LogicalLightStrip* PianoLocations::upRightLegRear(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::RIGHT_LEG_REAR_BOTTOM,
      PianoLocations::RIGHT_LEG_REAR_TOP);
}

