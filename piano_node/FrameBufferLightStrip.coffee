LightStrip = require("./LightStrip")
Colors = require("./Colors")

# TODO maybe split out PhysicalLightStrip and move the SPI/buffer
# stuff into that?

class FrameBufferLightStrip extends LightStrip
  constructor: (numPixels) ->
    super(numPixels)
    @pixels = new Array(numPixels)
    @_buffer = new Buffer(3 * numPixels + 1)
    @_buffer[@_buffer.length - 1] = 0
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

  buffer: () ->
    if @numPixels > 0
      for i in [(@numPixels - 1)...0] by -1
        @_buffer.writeUInt32BE(@pixels[i] | 0x808080, i * 3 - 1)
      firstPixel = @pixels[0] | 0x808080
      @_buffer[0] = (firstPixel >> 16) & 0xff
      @_buffer[1] = (firstPixel >> 8) & 0xff
      @_buffer[2] = (firstPixel >> 0) & 0xff
    @_buffer

module.exports = FrameBufferLightStrip
