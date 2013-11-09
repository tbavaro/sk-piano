# AmplitudeVisualizer parameters
AMPLITUDE_NOTE_INCREASE = 0.15
AMPLITUDE_DECAY_RATE = 0.3
AMPLITUDE_MAX = 1.2

# fraction of pixels to get new sparkles / second, when the piano is idle
SPARKLE_RATE_BASE = 0.03

# fraction of the value that decays per second for each sparkle
SPARKLE_DECAY_RATE = 6.0

DESATURATION_DECAY_RATE = 3.0

# degrees/sec
HUE_CYCLE_RATE = 45.0

# Sub-visualizer responsible for a particular region of the piano.
class TwinkleVisualizerHelper extends AmplitudeVisualizer
  constructor: (strip, pianoKeys, shouldHighlightKeys) ->
    super(pianoKeys, AMPLITUDE_NOTE_INCREASE, AMPLITUDE_DECAY_RATE, AMPLITUDE_MAX)
    @strip = strip
    @shouldHighlightKeys = shouldHighlightKeys

    # precalculate for each key
    @pixelForKey = (@calculatePixelForKey(key) for key in [1...PianoKeys.NUM_KEYS])

    @reset()

  reset: () ->
    @pixelValues = (0 for _ in [0...@strip.numPixels])
    @pixelDesaturations = (0 for _ in [0...@strip.numPixels])
    @sparkleAccum = 0
    @hue = 0
    super

  calculatePixelForKey: (key) ->
    Math.floor(1.0 * key / PianoKeys.NUM_KEYS * @strip.numPixels)

  renderValue: (rawValue, secondsSinceLastFrame) ->
    # we want to treat all values over 1.0 as 1.0 (gives it a bit of a 
    # lingering effect)
    value = Math.min(1.0, rawValue)

    # decay pixels
    sustain = Math.max(0.0, 1.0 - SPARKLE_DECAY_RATE * secondsSinceLastFrame)
    @pixelValues[i] *= sustain for i in [0...@pixelValues.length]

    # add new sparkles for this frame
    @addNewSparkles(value, secondsSinceLastFrame)

    # highlight keys if that's enabled for this helper
    @highlightKeys(secondsSinceLastFrame) if @shouldHighlightKeys

    # update the overall saturation and hue
    @overallSaturation = Math.min(1.0, Math.pow(value, 2.0))
    @hue = (@hue + HUE_CYCLE_RATE * secondsSinceLastFrame) % 360.0

    # draw
    for i in [0...@strip.numPixels]
      if @shouldHighlightKeys
        saturation = Math.max(0, @overallSaturation - @pixelDesaturations[i])
        brightness =
            Math.min(1, @pixelValues[i] * 0.3 + @pixelDesaturations[i] * 0.7)
      else
        saturation = @overallSaturation
        brightness = Math.min(1, @pixelValues[i])

      @strip.setPixel(i, Colors.hsb(@hue, saturation, brightness))

    return

  addNewSparkles: (value, secondsSinceLastFrame) ->
    sparkleIncreaseFraction =
      (SPARKLE_RATE_BASE + (Math.pow(value, 0.8) * 1.5)) * secondsSinceLastFrame
    @sparkleAccum += @strip.numPixels * sparkleIncreaseFraction
    while @sparkleAccum >= 1.0
      pixel = Math.floor(Math.random() * @strip.numPixels)
      @pixelValues[pixel] = 1.0
      @sparkleAccum -= 1.0

  highlightKeys: (secondsSinceLastFrame) ->
    # decay saturations
    desaturationSustain = Math.max(0.0, 1.0 - DESATURATION_DECAY_RATE * secondsSinceLastFrame)
    for i in [0...@pixelDesaturations.length]
      @pixelDesaturations[i] *= desaturationSustain

    # set saturations for pressed keys
    radius = Math.floor(2.0 * @strip.numPixels / PianoKeys.NUM_KEYS)
    for key in @pianoKeys.pressedKeys
      pixel = @pixelForKey[key]
      leftPixel = Math.max(0, pixel - radius)
      rightPixel = Math.min(@strip.numPixels - 1, pixel + radius)
      @pixelDesaturations[i] = 1.0 for i in [leftPixel..rightPixel]

# This is the actual {Visualizer} instance
class TwinkleVisualizer extends CompositeVisualizer
  constructor: (strip, pianoKeys) ->
    super([
      new TwinkleVisualizerHelper(strip, pianoKeys, false),
      new TwinkleVisualizerHelper(
          new LogicalLightStrip(strip, LEDRanges.TOP_FRONT_ROW),
          pianoKeys,
          true),
      new TwinkleVisualizerHelper(
          new LogicalLightStrip(strip, LEDRanges.DIRECTLY_ABOVE_KEYS),
          pianoKeys,
          true)
    ])
