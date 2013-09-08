LightStrip = require("./LightStrip")
Colors = require("./Colors")

class LogicalLightStrip extends LightStrip
  constructor: (delegate, pixel_mapping) ->
    super(pixel_mapping.length)
    @delegate = delegate
    @pixel_mapping = pixel_mapping

  reset: () ->
    @delegate.setPixel(i, Colors.BLACK) for i in @pixel_mapping

  getPixel: (i) ->
    if i < 0 || i >= @num_pixels \
        then Colors.BLACK \
        else @delegate.getPixel(@pixel_mapping[i])

  setPixel: (i, c) ->
    if i >= 0 && i < @num_pixels then @delegate.setPixel(@pixel_mapping[i], c)
    return

module.exports = LogicalLightStrip