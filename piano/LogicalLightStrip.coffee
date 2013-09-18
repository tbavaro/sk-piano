LightStrip = require("./LightStrip")
Colors = require("./Colors")

class LogicalLightStrip extends LightStrip
  constructor: (delegate, pixelMapping) ->
    super(pixelMapping.length)
    @delegate = delegate
    @pixelMapping = pixelMapping

  reset: () ->
    @delegate.setPixel(i, Colors.BLACK) for i in @pixelMapping
    return

  getPixel: (i) ->
    if i < 0 || i >= @numPixels
      Colors.BLACK
    else
      @delegate.getPixel(@pixelMapping[i])

  setPixel: (i, c) ->
    if i >= 0 && i < @numPixels
      pixel = @pixelMapping[i]
      if pixel != -1
        @delegate.setPixel(pixel, c)
    return

module.exports = LogicalLightStrip