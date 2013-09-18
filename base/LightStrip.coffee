Colors = require("Colors")

class LightStrip
  constructor: (numPixels) ->
    @numPixels = numPixels

  reset: () ->
    for i in [0...@numPixels] by 1
      @setPixel(i, Colors.BLACK)
    return

  getPixel: (i) -> throw "abstract"

  setPixel: (i, color) -> throw "abstract"

module.exports = LightStrip
