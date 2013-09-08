LightStrip = require("./LightStrip")
Colors = require("./Colors")

class FrameBufferLightStrip extends LightStrip
  constructor: (num_pixels) ->
    super(num_pixels)
    @_pixels = new Array(@num_pixels)
    this.reset()

  reset: () ->
    i = 0
    while i < @num_pixels
      @_pixels[i] = Colors.BLACK
      ++i
    return

  pixels: () -> @_pixels

  getPixel: (i) ->
    if (i < 0 || i >= @num_pixels) then Colors.BLACK else @_pixels[i]

  setPixel: (i, color) ->
    if (i >= 0 && i < @num_pixels) then @_pixels[i] = color
    return

module.exports = FrameBufferLightStrip
