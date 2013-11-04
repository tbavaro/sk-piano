Colors = require("./Colors")

# Represents a sequence of LED pixels.
# @author tbavaro
module.exports = class LightStrip
  # Constructs a new {LightStrip}.
  # @param {Integer} numPixels the number of pixels in the strip
  # @return {LightStrip}
  constructor: (numPixels) ->
    @numPixels = numPixels

  # Resets the strip back to all black pixels.
  reset: () ->
    for i in [0...@numPixels] by 1
      @setPixel(i, Colors.BLACK)
    return

  # Gets the color of the pixel at the given index.
  # @param {Integer} index the pixel index
  # @return {Color}
  # @abstract
  getPixel: (index) -> throw "abstract"

  # Sets the color of the pixel at the given index.
  # @param {Integer} index the pixel index
  # @param {Color} color the color to set
  # @abstract
  setPixel: (index, color) -> throw "abstract"
