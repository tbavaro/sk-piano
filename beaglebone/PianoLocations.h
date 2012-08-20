#ifndef __INCLUDED_PIANO_LOCATIONS_H
#define __INCLUDED_PIANO_LOCATIONS_H

#include "SKTypes.h"
#include "LogicalLightStrip.h"

/**
 * "Front" means the side where the player sits.
 * "Behind X" means further from the player than X.
 * "Hinge" refers to the hinge on the lid of the piano that allows you to
 *     fold back the top to reveal the strings.
 * "Second row" refers to the row of lights right below "top"
 * "Above keys" refers to the lights directly above the keyboard, not counting
 *     the lights on the front that are beyond the keyboard to the left or right
 */

class PianoLocations {
  public:
    /*** Individual LED numbers: ***/

    static const Pixel TOP_LEFT_JUST_BEHIND_HINGE = 0;
    static const Pixel TOP_RIGHT_JUST_BEHIND_HINGE = 79;
    static const Pixel TOP_RIGHT_JUST_IN_FRONT_OF_HINGE = 80;
    static const Pixel TOP_FRONT_ROW_RIGHT = 92;
    static const Pixel TOP_FRONT_ROW_LEFT = 135;
    static const Pixel TOP_LEFT_JUST_IN_FRONT_OF_HINGE = 147;
    static const Pixel SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 148;
    static const Pixel SECOND_ROW_LEFTMOST_ABOVE_KEYS = 163;
    static const Pixel SECOND_ROW_RIGHTMOST_ABOVE_KEYS = 203;
    static const Pixel SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE = 218;
    static const Pixel SECOND_ROW_RIGHT_JUST_BEHIND_HINGE = 219;
    static const Pixel SECOND_ROW_LEFT_JUST_BEHIND_HINGE = 297;
    // CHECK THIS: notes said "right" not "left" but that doesnt make sense
    static const Pixel THIRD_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 298;
    static const Pixel THIRD_ROW_FRONT_LEFT = 320;
    static const Pixel THIRD_ROW_FRONT_RIGHT = 363;
    // missing THIRD_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE
    // missing THIRD_ROW_RIGHT_JUST_BEHIND_HINGE
    static const Pixel THIRD_ROW_LEFT_JUST_BEHIND_HINGE = 457;
    static const Pixel BOTTOM_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 458;
    static const Pixel BOTTOM_ROW_FRONT_LEFT = 478;
    static const Pixel BOTTOM_ROW_FRONT_RIGHT = 521;
    static const Pixel BOTTOM_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE = 541;
    static const Pixel RIGHT_LEG_REAR_BOTTOM = 542;
    static const Pixel RIGHT_LEG_REAR_TOP = 559;
    static const Pixel RIGHT_LEG_FRONT_TOP = 560;
    static const Pixel RIGHT_LEG_FRONT_BOTTOM = 577;
    // missing BACK_LEG_RIGHT_FRONT_TOP
    // missing BACK_LEG_RIGHT_FRONT_BOTTOM
    // missing BACK_LEG_RIGHT_REAR_BOTTOM
    // missing BACK_LEG_RIGHT_REAR_TOP
    // missing BACK_LEG_LEFT_REAR_TOP
    // missing BACK_LEG_LEFT_REAR_BOTTOM
    // missing BACK_LEG_LEFT_FRONT_BOTTOM
    // missing BACK_LEG_LEFT_FRONT_TOP
    // missing LEFT_LEG_REAR_BOTTOM
    // missing LEFT_LEG_REAR_TOP
    // missing LEFT_LEG_FRONT_TOP
    // missing LEFT_LEG_FRONT_BOTTOM

  public:
    /*** Light Strip Regions ***/
    static LogicalLightStrip* directlyAbovePixels(LightStrip& delegate);
};

#endif

