L = require("./LEDLocations.coffee")

# "Front" means the side where the player sits.
# "Behind X" means further from the player than X.
# "Hinge" refers to the hinge on the lid of the piano that allows you to
#     fold back the top to reveal the strings.
# "Second row" refers to the row of lights right below "top"
# "Above keys" refers to the lights directly above the keyboard, not counting
#     the lights on the front that are beyond the keyboard to the left or right

ENTIRE_TOP =
    [].concat(
        [L.TOP_FRONT_ROW_LEFT_SIDE..L.TOP_LEFT_JUST_IN_FRONT_OF_HINGE],
        [L.TOP_LEFT_JUST_BEHIND_HINGE..L.TOP_RIGHT_JUST_BEHIND_HINGE],
        [L.TOP_RIGHT_JUST_IN_FRONT_OF_HINGE..L.TOP_FRONT_ROW_RIGHT_SIDE],
        [L.TOP_FRONT_ROW_RIGHT..L.TOP_FRONT_ROW_LEFT])

AROUND_TOP_EXCLUDING_FRONT_ROW =
    [].concat(
      [L.TOP_FRONT_ROW_LEFT_SIDE..L.TOP_LEFT_JUST_IN_FRONT_OF_HINGE],
      [L.TOP_LEFT_JUST_BEHIND_HINGE..L.TOP_RIGHT_JUST_BEHIND_HINGE],
      [L.TOP_RIGHT_JUST_IN_FRONT_OF_HINGE..L.TOP_FRONT_ROW_RIGHT_SIDE])

ENTIRE_SECOND_ROW =
    [].concat(
      [L.SECOND_ROW_FRONT_ROW_LEFT_SIDE..L.SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE],
      [L.SECOND_ROW_LEFT_JUST_BEHIND_HINGE..L.SECOND_ROW_RIGHT_JUST_BEHIND_HINGE],
      [L.SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE..L.SECOND_ROW_FRONT_ROW_RIGHT_SIDE],
      [L.SECOND_ROW_FRONT_ROW_RIGHT..L.SECOND_ROW_FRONT_ROW_LEFT])

AROUND_SECOND_ROW_EXCLUDING_FRONT_ROW =
    [].concat(
      [L.SECOND_ROW_FRONT_ROW_LEFT_SIDE..L.SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE],
      [L.SECOND_ROW_LEFT_JUST_BEHIND_HINGE..L.SECOND_ROW_RIGHT_JUST_BEHIND_HINGE],
      [L.SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE..L.SECOND_ROW_FRONT_ROW_RIGHT_SIDE])

AROUND_THIRD_ROW_EXCLUDING_FRONT_ROW =
    [].concat(
      [L.THIRD_ROW_FRONT_LEFT_SIDE..L.THIRD_ROW_LEFT_JUST_IN_FRONT_OF_HINGE],
      [L.THIRD_ROW_LEFT_JUST_BEHIND_HINGE..L.THIRD_ROW_FRONT_RIGHT_SIDE])

AROUND_BOTTOM_ROW_WITH_GAP_TO_MATCH_THIRD_ROW =
    [].concat(
      [L.BOTTOM_ROW_FRONT_LEFT_SIDE..L.BOTTOM_ROW_LEFT_JUST_IN_FRONT_OF_HINGE],
      -1 for _ in [0...76],
      [L.BOTTOM_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE..L.BOTTOM_ROW_FRONT_RIGHT_SIDE])

# LED pixel index ranges for specific ranges on the piano.  Particularly useful
# with {LogicalLightStrip}.
# @see LEDLocations
# @see LogicalLightStrip
# @author tbavaro
module.exports = class LEDRanges
  # @property {Array<Integer>} across the top front row, from left to right
  @TOP_FRONT_ROW:
      [L.TOP_FRONT_ROW_LEFT..L.TOP_FRONT_ROW_RIGHT]

  # @property {Array<Integer>} across the second front row (directly above the keys), from left to right
  @DIRECTLY_ABOVE_KEYS:
      [L.SECOND_ROW_LEFTMOST_ABOVE_KEYS..L.SECOND_ROW_RIGHTMOST_ABOVE_KEYS]

  # @property {Array<Integer>} across the third front row (directly below the keys), from left to right
  @THIRD_FRONT_ROW:
      [L.THIRD_ROW_FRONT_LEFT..L.THIRD_ROW_FRONT_RIGHT]

  # @property {Array<Integer>} across the bottom front row, from left to right
  @BOTTOM_FRONT_ROW:
      [L.BOTTOM_ROW_FRONT_LEFT..L.BOTTOM_ROW_FRONT_RIGHT]

  # @property {Array<Integer>} all the way around the top, starting at the front left LED, going all the way around the back of the piano, and then across the top row from right to left
  @ENTIRE_TOP: ENTIRE_TOP

  # @property {Array<Integer>} around the top, starting at the front left LED, going all the way around the back of the piano to the front right LED
  @AROUND_TOP_EXCLUDING_FRONT_ROW: AROUND_TOP_EXCLUDING_FRONT_ROW

  # @property {Array<Integer>} all the way around the second row, starting at the front left LED, going all the way around the back of the piano, and then across the top row from right to left
  @ENTIRE_SECOND_ROW: ENTIRE_SECOND_ROW

  # @property {Array<Integer>} around the second row, starting at the front left LED, going all the way around the back of the piano to the front right LED
  @AROUND_SECOND_ROW_EXCLUDING_FRONT_ROW: AROUND_SECOND_ROW_EXCLUDING_FRONT_ROW

  # @property {Array<Integer>} around the third row, starting at the front left LED, going all the way around the back of the piano to the front right LED
  @AROUND_THIRD_ROW_EXCLUDING_FRONT_ROW: AROUND_THIRD_ROW_EXCLUDING_FRONT_ROW

  # @property {Array<Integer>} around the bottom row, starting at the front left LED, going all the way around the back of the piano to the front right LED.  This has a gap (where setting pixels will do nothing) to match the 3rd row.
  @AROUND_BOTTOM_ROW_WITH_GAP_TO_MATCH_THIRD_ROW:
      AROUND_BOTTOM_ROW_WITH_GAP_TO_MATCH_THIRD_ROW
