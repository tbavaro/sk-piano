FrameBufferLightStrip = require("./FrameBufferLightStrip")

class PhysicalLightStrip extends FrameBufferLightStrip
  constructor: (spi, numPixels) ->
    @spi = spi
    @_buffer = new Buffer(3 * numPixels + 1)
    @_buffer[@_buffer.length - 1] = 0
    super(numPixels)

  display: () ->
    @spi.send(@_updateBuffer())
    return

  _updateBuffer: () ->
    if @numPixels > 0
      for i in [(@numPixels - 1)...0] by -1
        @_buffer.writeUInt32BE(@pixels[i] | 0x808080, i * 3 - 1)
      firstPixel = @pixels[0] | 0x808080
      @_buffer[0] = (firstPixel >> 16) & 0xff
      @_buffer[1] = (firstPixel >> 8) & 0xff
      @_buffer[2] = (firstPixel >> 0) & 0xff
    @_buffer

module.exports = PhysicalLightStrip
