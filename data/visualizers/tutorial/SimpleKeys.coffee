#
# Simple visualizer to demonstrate reading the pressed
# keys.  The color changes only while a key is pressed.
#

SPEED = 720 # degrees of hue per second

class SimpleKeys extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys
    @hue = 0

  render: (secondsSinceLastFrame) ->
    # update hue if a key is pressed
    if (@pianoKeys.pressedKeys.length > 0)
      @hue = (@hue + secondsSinceLastFrame * SPEED) % 360.0

    saturation = 1
    brightness = 1

    color = Colors.hsb(@hue, saturation, brightness)

    # set the entire strip to the same color
    @strip.setPixel(i, color) for i in [0...@strip.numPixels]
