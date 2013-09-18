LightStrip = require("./LightStrip")
Colors = require("./Colors")

class FrameBufferLightStrip extends LightStrip
  constructor: (numPixels) ->
    super(numPixels)
    @pixels = new Array(numPixels)
    @reset()

  reset: () ->
    for i in [0...@numPixels] by 1
      @pixels[i] = Colors.BLACK
    return

  getPixel: (i) ->
    @pixels[i]

  setPixel: (i, color) ->
    @pixels[i] = color
    return

module.exports = FrameBufferLightStrip
