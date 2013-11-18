#
# Simple visualizer to demonstrate setting colors and depending
# upon time.  The colors cycle endlessly; notes don't do anything.
#

SPEED = 360 # degrees of hue per second
RAINBOW_LENGTH = 20 # pixels to get through the whole rainbow

# calculated from above
RAINBOW_DENSITY = 360 / RAINBOW_LENGTH

class SimpleRainbow extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys
    @phase = 0

  render: (secondsSinceLastFrame) ->
    # update phase; it's good to keep values from growing too large
    @phase = (@phase + secondsSinceLastFrame * SPEED) % 360.0

    saturation = 1
    brightness = 1

    for i in [0...@strip.numPixels]
      hue = (i * RAINBOW_DENSITY + @phase) % 360.0
      @strip.setPixel(i, Colors.hsb(hue, saturation, brightness))
