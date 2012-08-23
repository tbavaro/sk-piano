#include "PianoLocations.h"

using namespace std;

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

LogicalLightStrip* PianoLocations::entireTop(LightStrip& delegate) {
  vector<PixelRange> ranges;
  ranges.push_back(
      PixelRange(PianoLocations::TOP_FRONT_ROW_LEFT_SIDE,
                 PianoLocations::TOP_LEFT_JUST_IN_FRONT_OF_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::TOP_LEFT_JUST_BEHIND_HINGE,
                 PianoLocations::TOP_RIGHT_JUST_BEHIND_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::TOP_RIGHT_JUST_IN_FRONT_OF_HINGE,
                 PianoLocations::TOP_FRONT_ROW_RIGHT_SIDE));
  ranges.push_back(
      PixelRange(PianoLocations::TOP_FRONT_ROW_RIGHT,
                 PianoLocations::TOP_FRONT_ROW_LEFT));
  return LogicalLightStrip::fromRanges(delegate, ranges);
}

LogicalLightStrip* PianoLocations::aroundTopExcludingFrontRow(LightStrip& delegate) {
  vector<PixelRange> ranges;
  ranges.push_back(
      PixelRange(PianoLocations::TOP_FRONT_ROW_LEFT_SIDE,
                 PianoLocations::TOP_LEFT_JUST_IN_FRONT_OF_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::TOP_LEFT_JUST_BEHIND_HINGE,
                 PianoLocations::TOP_RIGHT_JUST_BEHIND_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::TOP_RIGHT_JUST_IN_FRONT_OF_HINGE,
                 PianoLocations::TOP_FRONT_ROW_RIGHT_SIDE));
  return LogicalLightStrip::fromRanges(delegate, ranges);
}

LogicalLightStrip* PianoLocations::entireSecondRow(LightStrip& delegate) {
  vector<PixelRange> ranges;
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_FRONT_ROW_LEFT_SIDE,
                 PianoLocations::SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_LEFT_JUST_BEHIND_HINGE,
                 PianoLocations::SECOND_ROW_RIGHT_JUST_BEHIND_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE,
                 PianoLocations::SECOND_ROW_FRONT_ROW_RIGHT_SIDE));
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_FRONT_ROW_RIGHT,
                 PianoLocations::SECOND_ROW_FRONT_ROW_LEFT));
  return LogicalLightStrip::fromRanges(delegate, ranges);
}

LogicalLightStrip* PianoLocations::aroundSecondRowExcludingFrontRow(LightStrip& delegate) {
  vector<PixelRange> ranges;
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_FRONT_ROW_LEFT_SIDE,
                 PianoLocations::SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_LEFT_JUST_BEHIND_HINGE,
                 PianoLocations::SECOND_ROW_RIGHT_JUST_BEHIND_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE,
                 PianoLocations::SECOND_ROW_FRONT_ROW_RIGHT_SIDE));
  return LogicalLightStrip::fromRanges(delegate, ranges);
}

