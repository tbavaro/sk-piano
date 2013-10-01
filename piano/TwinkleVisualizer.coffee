AmplitudeVisualizer = require("./AmplitudeVisualizer")
Colors = require("./Colors")
CompositeVisualizer = require("./CompositeVisualizer")
LogicalLightStrip = require("./LogicalLightStrip")
PianoKeys = require("./PianoKeys")
PianoLocations = require("./PianoLocations")

SPARKLE_RATE_BASE = 0.001

# TODO make this FPS-independent
# TODO clean up the magic numbers everywhere

class TwinkleVisualizerHelper extends AmplitudeVisualizer
  constructor: (strip, pianoKeys, highlightKeys) ->
    super(pianoKeys, 0.15, 0.3, 1.2)
    @strip = strip
    @highlightKeys = highlightKeys
    @reset()

  reset: () ->
    @pixelValues = (0 for _ in [0...@strip.numPixels] by 1)
    @pixelSaturations = (0 for _ in [0...@strip.numPixels] by 1)
    @sparkleAccum = 0
    super

  # TODO it probably makes sense to precalculate this in the constructor
  pixelForKey: (key) ->
    Math.floor(1.0 * key / PianoKeys.NUM_KEYS * @strip.numPixels)

  renderValue: (value, secondsSinceLastFrame) ->
    value = Math.min(1.0, value)

    # decay pixels
    @pixelValues[i] *= 0.8 for i in [0...@pixelValues.length] by 1

    # reset some pixels to 1
    @sparkleAccum +=
      @strip.numPixels * (SPARKLE_RATE_BASE + (Math.pow(value, 0.8) * 0.05))
    while @sparkleAccum >= 1.0
      pixel = Math.floor(Math.random() * @strip.numPixels)
      @pixelValues[pixel] = 1.0
      @sparkleAccum -= 1.0

    if @highlightKeys
      # decay saturations
      @pixelSaturations[i] *= 0.9 for i in [0...@pixelSaturations.length] by 1

      # set saturations for pressed keys
      radius = Math.floor(2.0 * @strip.numPixels / PianoKeys.NUM_KEYS)
      for key in @pianoKeys.pressedKeys
        pixel = @pixelForKey(key)
        leftPixel = Math.max(0, pixel - radius)
        rightPixel = Math.min(@strip.numPixels - 1, pixel + radius)
        @pixelSaturations[i] = 1.0 for i in [leftPixel..rightPixel] by 1

    # draw
    overallSaturation = Math.min(1.0, Math.pow(value, 2.0))
    hue = (Date.now() / 30.0) % 360.0
    for i in [0...@strip.numPixels] by 1
      if @highlightKeys
        saturation = Math.max(0, overallSaturation - @pixelSaturations[i])
        brightness =
            Math.min(1, @pixelValues[i] * 0.3 + @pixelSaturations[i] * 0.7)
      else
        saturation = overallSaturation
        brightness = Math.min(1, @pixelValues[i])

      @strip.setPixel(i, Colors.hsv(hue, saturation, brightness))


    return

class TwinkleVisualizer extends CompositeVisualizer
  constructor: (strip, pianoKeys) ->
    super([
      new TwinkleVisualizerHelper(strip, pianoKeys, false),
      new TwinkleVisualizerHelper(
        new LogicalLightStrip(strip, PianoLocations.top_front_row),
        pianoKeys,
        true),
      new TwinkleVisualizerHelper(
        new LogicalLightStrip(strip, PianoLocations.directly_above_keys),
        pianoKeys,
        true)
    ])

  toString: () -> "TwinkleVisualizer"

module.exports = TwinkleVisualizer
