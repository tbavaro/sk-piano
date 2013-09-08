# "Front" means the side where the player sits.
# "Behind X" means further from the player than X.
# "Hinge" refers to the hinge on the lid of the piano that allows you to
#     fold back the top to reveal the strings.
# "Second row" refers to the row of lights right below "top"
# "Above keys" refers to the lights directly above the keyboard, not counting
#     the lights on the front that are beyond the keyboard to the left or right

module PianoLocations
  # Individual LED numbers:
  TOP_LEFT_JUST_BEHIND_HINGE = 0
  TOP_RIGHT_JUST_BEHIND_HINGE = 79
  TOP_RIGHT_JUST_IN_FRONT_OF_HINGE = 80
  TOP_FRONT_ROW_RIGHT_SIDE = 91
  TOP_FRONT_ROW_RIGHT = 92
  TOP_FRONT_ROW_LEFT = 135
  TOP_FRONT_ROW_LEFT_SIDE = 136
  TOP_LEFT_JUST_IN_FRONT_OF_HINGE = 147
  SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 148
  SECOND_ROW_FRONT_ROW_LEFT_SIDE = 158
  SECOND_ROW_FRONT_ROW_LEFT = 159
  SECOND_ROW_LEFTMOST_ABOVE_KEYS = 163
  SECOND_ROW_RIGHTMOST_ABOVE_KEYS = 203
  SECOND_ROW_FRONT_ROW_RIGHT = 207
  SECOND_ROW_FRONT_ROW_RIGHT_SIDE = 208
  SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE = 218
  SECOND_ROW_RIGHT_JUST_BEHIND_HINGE = 219
  SECOND_ROW_LEFT_JUST_BEHIND_HINGE = 297
  THIRD_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 298
  THIRD_ROW_FRONT_LEFT_SIDE = 319
  THIRD_ROW_FRONT_LEFT = 320
  THIRD_ROW_FRONT_RIGHT = 363
  THIRD_ROW_FRONT_RIGHT_SIDE = 364
  # missing THIRD_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE
  # missing THIRD_ROW_RIGHT_JUST_BEHIND_HINGE
  THIRD_ROW_LEFT_JUST_BEHIND_HINGE = 457
  BOTTOM_ROW_LEFT_JUST_IN_FRONT_OF_HINGE = 458
  BOTTOM_ROW_FRONT_LEFT_SIDE = 477
  BOTTOM_ROW_FRONT_LEFT = 478
  BOTTOM_ROW_FRONT_RIGHT = 521
  BOTTOM_ROW_FRONT_RIGHT_SIDE = 522
  BOTTOM_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE = 541

  def PianoLocations._make_range(first, last)
    (first < last) ? first.upto(last).to_a : first.downto(last).to_a
  end

  # light strip ranges
  def PianoLocations.top_front_row
    _make_range(TOP_FRONT_ROW_LEFT, TOP_FRONT_ROW_RIGHT)
  end

  def PianoLocations.directly_above_keys
    _make_range(SECOND_ROW_LEFTMOST_ABOVE_KEYS, SECOND_ROW_RIGHTMOST_ABOVE_KEYS)
  end

  def PianoLocations.third_front_row
    _make_range(THIRD_ROW_FRONT_LEFT, THIRD_ROW_FRONT_RIGHT)
  end

  def PianoLocations.bottom_front_row
    _make_range(BOTTOM_ROW_FRONT_LEFT, BOTTOM_ROW_FRONT_RIGHT)
  end

  def PianoLocations.entire_top
    [
      _make_range(TOP_FRONT_ROW_LEFT_SIDE, TOP_LEFT_JUST_IN_FRONT_OF_HINGE),
      _make_range(TOP_LEFT_JUST_BEHIND_HINGE, TOP_RIGHT_JUST_BEHIND_HINGE),
      _make_range(TOP_RIGHT_JUST_IN_FRONT_OF_HINGE, TOP_FRONT_ROW_RIGHT_SIDE),
      _make_range(TOP_FRONT_ROW_RIGHT, TOP_FRONT_ROW_LEFT)
    ].flatten
  end

  def PianoLocations.around_top_excluding_front_row
    [
      _make_range(TOP_FRONT_ROW_LEFT_SIDE, TOP_LEFT_JUST_IN_FRONT_OF_HINGE),
      _make_range(TOP_LEFT_JUST_BEHIND_HINGE, TOP_RIGHT_JUST_BEHIND_HINGE),
      _make_range(TOP_RIGHT_JUST_IN_FRONT_OF_HINGE, TOP_FRONT_ROW_RIGHT_SIDE)
    ]
  end

  def PianoLocations.entire_second_row
    [
      _make_range(SECOND_ROW_FRONT_ROW_LEFT_SIDE, SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE),
      _make_range(SECOND_ROW_LEFT_JUST_BEHIND_HINGE, SECOND_ROW_RIGHT_JUST_BEHIND_HINGE),
      _make_range(SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE, SECOND_ROW_FRONT_ROW_RIGHT_SIDE),
      _make_range(SECOND_ROW_FRONT_ROW_RIGHT, SECOND_ROW_FRONT_ROW_LEFT)
    ]
  end

  def PianoLocations.around_second_row_excluding_front_row
    [
      _make_range(SECOND_ROW_FRONT_ROW_LEFT_SIDE, SECOND_ROW_LEFT_JUST_IN_FRONT_OF_HINGE),
      _make_range(SECOND_ROW_LEFT_JUST_BEHIND_HINGE, SECOND_ROW_RIGHT_JUST_BEHIND_HINGE),
      _make_range(SECOND_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE, SECOND_ROW_FRONT_ROW_RIGHT_SIDE)
    ]
  end

  def PianoLocations.around_third_row_excluding_front_row
    [
      _make_range(THIRD_ROW_FRONT_LEFT_SIDE, THIRD_ROW_LEFT_JUST_IN_FRONT_OF_HINGE),
      _make_range(THIRD_ROW_LEFT_JUST_BEHIND_HINGE, THIRD_ROW_FRONT_RIGHT_SIDE)
    ]
  end

  def PianoLocations.around_bttom_row_with_gap_to_match_third_row
    [
      _make_range(BOTTOM_ROW_FRONT_LEFT_SIDE, BOTTOM_ROW_LEFT_JUST_IN_FRONT_OF_HINGE),
      [-1] * 76,
      _make_range(BOTTOM_ROW_RIGHT_JUST_IN_FRONT_OF_HINGE, BOTTOM_ROW_FRONT_RIGHT_SIDE)
    ]
  end
end
