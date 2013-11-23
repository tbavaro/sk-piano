#
# Starts dark. Every time you press a key, it gains in brightness and 
# hues from cool to warm.
#
DECAY_RATE = 2.0
STARTING_HUE = 180
HUE_DECAY_COEFF = 50.5 # higher is slower decay

class HeatMapHelper extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys
    
    @pixelForKey = (@calculatePixelForKey(key) for key in [1...PianoKeys.NUM_KEYS])
    @reset()

  reset: () ->
    @pixelBrightness = (0 for _ in [0...@strip.numPixels])
    @pixelHue = (0 for _ in [0...@strip.numPixels])

  calculatePixelForKey: (key) ->
    Math.floor(1.0 * key / PianoKeys.NUM_KEYS * @strip.numPixels)
    
  updatePixelsInNeighborhood: (center_pixel, radius) ->
    lowerBound = Math.max(0, center_pixel - radius)
    upperBound = Math.min(@strip.numPixels, center_pixel + radius)
    for i in [lowerBound...upperBound + 1]
      fudge_factor = 1 - (Math.abs(i - center_pixel) / radius)
      if (@pixelBrightness[i] < 0.1)
        @pixelHue[i] = STARTING_HUE
      else
        @pixelHue[i] = Math.max(0, @pixelHue[i] - 7)
      @pixelBrightness[i] = Math.max(@pixelBrightness[i], 1 * fudge_factor)
    
    
  render: (secondsSinceLastFrame) ->
    for key in @pianoKeys.pressedKeys
      pixel = @pixelForKey[key]
      @updatePixelsInNeighborhood(pixel, 4)

    # decay pixels
    sustain = Math.max(0.0, 1.0 - DECAY_RATE * secondsSinceLastFrame)
    for i in [0...@pixelBrightness.length]      
      @pixelBrightness[i] *= sustain 
      # weighted avg of current hue and base hue
      @pixelHue[i] = Math.round(
        (STARTING_HUE + HUE_DECAY_COEFF * @pixelHue[i]) /
        (1 + HUE_DECAY_COEFF))
    
    # draw
    for i in [0...@strip.numPixels]
      saturation = 1.0
      brightness = Math.min(1, @pixelBrightness[i])
      
      @strip.setPixel(i, Colors.hsb(@pixelHue[i], saturation, brightness))
      
      
class HeatMap extends CompositeVisualizer
  constructor: (strip, pianoKeys) ->
    super([
      # new HeatMapHelper(strip, pianoKeys, false),
      new HeatMapHelper(
          new LogicalLightStrip(strip, LEDRanges.TOP_FRONT_ROW),
          pianoKeys,
          true),
      new HeatMapHelper(
          new LogicalLightStrip(strip, LEDRanges.DIRECTLY_ABOVE_KEYS),
          pianoKeys,
          true)
    ])
