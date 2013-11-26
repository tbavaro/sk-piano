#
# The colors cycle endlessly; Notes make the colors spin faster
#

RAINBOW_LENGTH = 20 # pixels to get through the whole rainbow

# calculated from above
RAINBOW_DENSITY = 360 / RAINBOW_LENGTH

class SpeedyRainbow extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys
    @phase = 0
    @speed = 0

  render: (secondsSinceLastFrame) ->
  
    if (@pianoKeys.pressedSinceLastFrame.length > 0)
      @speed = Math.min(2000, (@speed + (@pianoKeys.pressedSinceLastFrame.length * 10)))
    else
      @speed = Math.max(-100, @speed-5)

  
    # update phase; it's good to keep values from growing too large
    @phase = (@phase + secondsSinceLastFrame * @speed) % 360.0

    saturation = 1
    brightness = 1

    for i in [0...@strip.numPixels]
      hue = (i * RAINBOW_DENSITY + @phase) % 360.0
      @strip.setPixel(i, Colors.hsb(hue, saturation, brightness))
