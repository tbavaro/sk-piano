#
# Simple visualizer to demonstrate reading the pressed
# keys and reacting to their location.
#
# This also uses LogicalLightStrip to only use the LEDs
# directly above the keys.
#

SPEED = 10 # pixels per second to move

MIDDLE_C = 39

class SimpleKeys2 extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = new LogicalLightStrip(strip, LEDRanges.DIRECTLY_ABOVE_KEYS)
    @pianoKeys = pianoKeys
    @maxPosition = @strip.numPixels - 1
    @position = @maxPosition / 2

  firstKey: () ->
    pressedKeys = @pianoKeys.pressedKeys
    if pressedKeys.length == 0 then null else pressedKeys[0]

  render: (secondsSinceLastFrame) ->
    key = @firstKey()

    # set direction; -1 means go left, 1 means go right
    if key == null
      direction = 0
    else if key < MIDDLE_C
      direction = -1
    else
      direction = 1

    # move 'position' but make sure we stay in bounds
    @position += direction * SPEED * secondsSinceLastFrame
    if @position < 0
      @position = 0
    else if @position > @maxPosition
      @position = @maxPosition

    # turn off all LEDs and light up only the one at 'position'
    @strip.reset()
    @strip.setPixel(Math.round(@position), Colors.WHITE)
