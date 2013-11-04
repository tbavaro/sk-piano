LightStrip = require("./LightStrip")
Colors = require("./Colors")

# A {LightStrip} abstraction mapping logical pixel locations onto another light
# strip.
#
# @example Create a {LightStrip} that only contains pixels along the top front row
#    frontRowStrip = new LogicalLightStrip(parentStrip, LEDRanges.TOP_FRONT_ROW)
# @author tbavaro
module.exports = class LogicalLightStrip extends LightStrip
  # Constructs a {LogicalLightStrip}.
  # @param {LightStrip} parentStrip the parent {LightStrip} that we are mapping onto
  # @param {Array<Integer>} pixelMapping ordered array of parent {LightStrip} pixel indices
  # @return {LogicalLightStrip}
  constructor: (parentStrip, pixelMapping) ->
    super(pixelMapping.length)
    @parentStrip = parentStrip
    @pixelMapping = pixelMapping

  # Resets the strip back to all black pixels.
  reset: () ->
    @parentStrip.setPixel(i, Colors.BLACK) for i in @pixelMapping
    return

  # Gets the color of the pixel at the given logical index.
  # @param {Integer} index the logical pixel index
  # @return {Color} the color at that logical pixel index, or black if it is out of range or unmapped
  getPixel: (index) ->
    parentIndex =
        (if index < 0 || index >= @numPixels then @pixelMapping[index] else -1)
    if parentIndex == -1
      Colors.BLACK
    else
      @parentStrip.getPixel(parentIndex)

  # Sets the color of the pixel at the given logical index.
  # @param {Integer} index the logical pixel index
  # @param {Color} color the color to set
  setPixel: (index, color) ->
    if index >= 0 && index < @numPixels
      parentIndex = @pixelMapping[index]
      if parentIndex != -1
        @parentStrip.setPixel(parentIndex, color)
    return
