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
    
LogicalLightStrip* PianoLocations::thirdFrontRow(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::THIRD_ROW_FRONT_LEFT,
      PianoLocations::THIRD_ROW_FRONT_RIGHT);
}

LogicalLightStrip* PianoLocations::bottomFrontRow(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::BOTTOM_ROW_FRONT_LEFT,
      PianoLocations::BOTTOM_ROW_FRONT_RIGHT);
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

LogicalLightStrip* PianoLocations::upBackLegFrontRight(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::BACK_LEG_FRONT_RIGHT_BOTTOM,
      PianoLocations::BACK_LEG_FRONT_RIGHT_TOP);
}
LogicalLightStrip* PianoLocations::upBackLegRearRight(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::BACK_LEG_REAR_RIGHT_BOTTOM,
      PianoLocations::BACK_LEG_REAR_RIGHT_TOP);
}
LogicalLightStrip* PianoLocations::upBackLegFrontLeft(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::BACK_LEG_FRONT_LEFT_BOTTOM,
      PianoLocations::BACK_LEG_FRONT_LEFT_TOP);
}
LogicalLightStrip* PianoLocations::upBackLegRearLeft(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::BACK_LEG_REAR_LEFT_BOTTOM,
      PianoLocations::BACK_LEG_REAR_LEFT_TOP);
}
LogicalLightStrip* PianoLocations::upLeftLegFront(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::LEFT_LEG_FRONT_BOTTOM,
      PianoLocations::LEFT_LEG_FRONT_TOP);
}
LogicalLightStrip* PianoLocations::upLeftLegRear(LightStrip& delegate) {
  return LogicalLightStrip::fromRange(
      delegate, 
      PianoLocations::LEFT_LEG_REAR_BOTTOM,
      PianoLocations::LEFT_LEG_REAR_TOP);
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

LogicalLightStrip* PianoLocations::aroundThirdRowExcludingFrontRow(LightStrip& delegate) {
  vector<PixelRange> ranges;
  ranges.push_back(
      PixelRange(PianoLocations::THIRD_ROW_FRONT_LEFT_SIDE,
                 PianoLocations::THIRD_ROW_LEFT_JUST_IN_FRONT_OF_HINGE));
  ranges.push_back(
      PixelRange(PianoLocations::THIRD_ROW_LEFT_JUST_BEHIND_HINGE,
                 PianoLocations::THIRD_ROW_FRONT_RIGHT_SIDE));
  return LogicalLightStrip::fromRanges(delegate, ranges);
}

LogicalLightStrip* PianoLocations::aroundBottomRowWithGapToMatchThirdRow(LightStrip& delegate) {
  vector<LogicalLightStrip*> strips;
  strips.push_back(LogicalLightStrip::fromRange(delegate, 
        BOTTOM_ROW_FRONT_LEFT_SIDE, BOTTOM_ROW_LEFT_JUST_IN_FRONT_OF_HINGE));
  strips.push_back(LogicalLightStrip::noopStrip(delegate, 76));
  strips.push_back(LogicalLightStrip::fromRange(delegate, 
        BOTTOM_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE, BOTTOM_ROW_FRONT_RIGHT_SIDE));

  LogicalLightStrip* ret = 
    LogicalLightStrip::joinLogicalStrips(delegate, strips);

  // clean up temp strips
  for (vector<LogicalLightStrip*>::const_iterator i = strips.begin();
       i != strips.end(); ++i) {
    delete *i;
  }

  return ret;
}

