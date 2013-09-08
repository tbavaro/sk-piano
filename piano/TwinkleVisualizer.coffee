AmplitudeVisualizer = require("./AmplitudeVisualizer")
Colors = require("./Colors")
CompositeVisualizer = require("./CompositeVisualizer")
LogicalLightStrip = require("./LogicalLightStrip")
PianoLocations = require("./PianoLocations")

class TwinkleVisualizerHelper extends AmplitudeVisualizer
  constructor: (light_strip, highlight_keys) ->
    super(light_strip, 0.15, 0.3, 1.2)
    @light_strip = light_strip
    @highlight_keys = highlight_keys
    this.reset()

  reset: () ->
    @pixel_values = 0 for _ in [0...@light_strip.numPixels()]
    @pixel_saturations = 0 for _ in [0...@light_strip.numPixels()]
    @pressed_keys = []
    @sparkle_accum = 0
    super

  pixelForKey: (key) ->
    Math.floor(key / 88.0 * @light_strip.numPixels())

  setPressedKeys: (pressed_keys) ->
    @pressed_keys = pressed_keys
    super

  renderValue: (value) ->
    value = Math.min(1.0, value)

    # decay pixels
    @pixel_values[i] *= 0.8 for i in [0...@pixel_values.length]

    # reset some pixels to 1
    @sparkle_accum +=
      @light_strip.num_pixels * (0.001 + (Math.pow(value, 0.8) * 0.05))
    while @sparkle_accum >= 1.0
      pixel = Math.floor(Math.random() * @light_strip.numPixels())
      @pixel_values[pixel] = 1.0
      @sparkle_accum -= 1.0

    if @highlight_keys
      # decay saturations
      @pixel_saturations[i] *= 0.9 for i in [0...@pixel_saturations.length]

      # set saturations for pressed keys
      radius = Math.floor(@light_strip.numPixels() / 88.0 * 2.0)
      @pressed_keys.forEach (key) ->
        pixel = this.pixelForKey(key)
        left_pixel = Math.max(0, pixel - radius)
        right_pixel = Math.min(@light_strip.numPixels() - 1, pixel + radius)
        @pixel_saturations[i] = 1.0 for i in [left_pixel..right_pixel]

    # draw
    overall_saturation = Math.min(1.0, Math.pow(value, 2.0))
    hue = (Date.now() / 30.0) % 360.0
    i = 0
    while i < @light_strip.numPixels()
      if @highlight_keys
        saturation = Math.max(0, overall_saturation - @pixel_saturations[i])
        brightness =
            Math.min(1, @pixel_values[i] * 0.3 + @pixel_saturations[i] * 0.7)
      else
        saturation = overall_saturation
        brightness = Math.min(1, @pixel_values[i])

      @light_strip.setPixel(i, Colors.hsv(hue, saturation, brightness))

      ++i

    return

class TwinkleVisualizer extends CompositeVisualizer
  constructor: (light_strip) ->
    super([
      new TwinkleVisualizerHelper(light_strip, false),
      new TwinkleVisualizerHelper(
        new LogicalLightStrip(light_strip, PianoLocations.top_front_row),
        true),
      new TwinkleVisualizerHelper(
        new LogicalLightStrip(light_strip, PianoLocations.directly_above_keys),
        true)
    ])

module.exports = TwinkleVisualizer
