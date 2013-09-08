Colors = require("./Colors")

class LightStrip
  constructor: (num_pixels) ->
    @num_pixels = num_pixels

  numPixels: () -> @num_pixels

  reset: () ->
    i = 0
    while i < @num_pixels
      this.setPixel(i, Colors.BLACK)
    return

  pixels: () ->
    while i < @num_pixels
      this.getPixel(i)

  setPixel: (i, color) ->
    throw "abstract"

  getPixel: (i, color) ->
    throw "abstract"

module.exports = LightStrip
