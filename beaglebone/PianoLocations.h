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

    enum {
      TOP_LEFT_JUST_BEHIND_HINGE = 0,
      TOP_RIGHT_JUST_BEHIND_HINGE = 79,
      TOP_RIGHT_JUST_IN_FRONT_OF_HINGE = 80,
      TOP_FRONT_ROW_RIGHT_SIDE = 91,
      TOP_FRONT_ROW_RIGHT = 92,
      TOP_FRONT_ROW_LEFT = 135,
      TOP_FRONT_ROW_LEFT_SIDE = 136,
      TOP_LEFT_JUST_IN_FRONT_OF_HINGE = 147,
      SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 148,
      SECOND_ROW_FRONT_ROW_LEFT_SIDE = 158,  // check this
      SECOND_ROW_FRONT_ROW_LEFT = 159,  // check this
      SECOND_ROW_LEFTMOST_ABOVE_KEYS = 163,
      SECOND_ROW_RIGHTMOST_ABOVE_KEYS = 203,
      SECOND_ROW_FRONT_ROW_RIGHT = 207,  // check this
      SECOND_ROW_FRONT_ROW_RIGHT_SIDE = 208,  // check this
      SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE = 218,
      SECOND_ROW_RIGHT_JUST_BEHIND_HINGE = 219,
      SECOND_ROW_LEFT_JUST_BEHIND_HINGE = 297,
      // CHECK THIS: notes said "right" not "left" but that doesnt make sense
      THIRD_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 298,
      THIRD_ROW_FRONT_LEFT_SIDE = 319,
      THIRD_ROW_FRONT_LEFT = 320,
      THIRD_ROW_FRONT_RIGHT = 363,
      THIRD_ROW_FRONT_RIGHT_SIDE = 364,
      // missing THIRD_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE
      // missing THIRD_ROW_RIGHT_JUST_BEHIND_HINGE
      THIRD_ROW_LEFT_JUST_BEHIND_HINGE = 457,
      BOTTOM_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 458,
      BOTTOM_ROW_FRONT_LEFT_SIDE = 477,
      BOTTOM_ROW_FRONT_LEFT = 478,
      BOTTOM_ROW_FRONT_RIGHT = 521,
      BOTTOM_ROW_FRONT_RIGHT_SIDE = 522,
      BOTTOM_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE = 541,
      RIGHT_LEG_REAR_BOTTOM = 542,
      RIGHT_LEG_REAR_TOP = 559,
      RIGHT_LEG_FRONT_TOP = 560,
      RIGHT_LEG_FRONT_BOTTOM = 577,
      BACK_LEG_FRONT_RIGHT_TOP = 578, // check this
      BACK_LEG_FRONT_RIGHT_BOTTOM = 595, // check this
      BACK_LEG_REAR_RIGHT_BOTTOM = 596, // check this
      BACK_LEG_REAR_RIGHT_TOP = 613, // check this
      BACK_LEG_REAR_LEFT_TOP = 614, // check this
      BACK_LEG_REAR_LEFT_BOTTOM = 631, // check this
      BACK_LEG_FRONT_LEFT_BOTTOM = 632, // check this
      BACK_LEG_FRONT_LEFT_TOP = 649, // check this
      LEFT_LEG_REAR_BOTTOM = 650, // check this
      LEFT_LEG_REAR_TOP = 667, // check this
      LEFT_LEG_FRONT_TOP = 668, // check this
      LEFT_LEG_FRONT_BOTTOM = 685 // check this
    };

  public:
    /*** Light Strip Regions ***/
    static LogicalLightStrip* topFrontRow(LightStrip& delegate);
    static LogicalLightStrip* directlyAboveKeys(LightStrip& delegate);
    static LogicalLightStrip* thirdFrontRow(LightStrip& delegate);
    static LogicalLightStrip* bottomFrontRow(LightStrip& delegate);


    static LogicalLightStrip* upRightLegFront(LightStrip& delegate);
    static LogicalLightStrip* upRightLegRear(LightStrip& delegate);
    static LogicalLightStrip* upBackLegFrontRight(LightStrip& delegate);
    static LogicalLightStrip* upBackLegRearRight(LightStrip& delegate);
    static LogicalLightStrip* upBackLegFrontLeft(LightStrip& delegate);
    static LogicalLightStrip* upBackLegRearLeft(LightStrip& delegate);
    static LogicalLightStrip* upLeftLegFront(LightStrip& delegate);
    static LogicalLightStrip* upLeftLegRear(LightStrip& delegate);

    /**
     * Entire top, starting at left front and going around the back all the way
     * back to left front.
     */
    static LogicalLightStrip* entireTop(LightStrip& delegate);

    /**
     * Same as entireTop but excludes the front row
     */
    static LogicalLightStrip* aroundTopExcludingFrontRow(LightStrip& delegate);
    
    /**
     * Entire second row, starting at left front and going around the back all 
     * the way back to left front.
     */
    static LogicalLightStrip* entireSecondRow(LightStrip& delegate);

    /**
     * Same as entireTop but excludes the front row
     */
    static LogicalLightStrip* aroundSecondRowExcludingFrontRow(LightStrip& delegate);

    /**
     * Same as entireThirdRow but excludes the front row
     */
    static LogicalLightStrip* aroundThirdRowExcludingFrontRow(LightStrip& delegate);

    /**
     * Along the bottom row, parallel to aroundThirdRowExcludingFrontRow, with
     * a gap in the strip to match the third row (pixels mapped to the gap will
     * not actually light any LEDs)
     */
    static LogicalLightStrip* aroundBottomRowWithGapToMatchThirdRow(LightStrip& delegate);
    
};

#endif

