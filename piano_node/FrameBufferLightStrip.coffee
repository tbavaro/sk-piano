LightStrip = require("./LightStrip")
Colors = require("./Colors")

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

#class FrameBufferLightStrip extends LightStrip
#  constructor: (numPixels) ->
#    super(numPixels)
#    @buffer = new Buffer(3 * @numPixels + 1)
#    @buffer[@buffer.length - 1] = 0
#    @reset()
#
#  reset: () ->
#    @buffer.fill(0x80, 0, @buffer.length - 1)
#    return
#
#  getPixel: (i) ->
#    (@buffer.readInt32BE(i * 3) >> 8) & 0x007f7f7f
#
#  setPixel: (i, color) ->
#    word = (((color | 0x808080) & 0xffffff) << 8) + @buffer.readUInt8(i * 3 + 3)
#    @buffer.writeInt32BE(word, i * 3)
#    return

module.exports = FrameBufferLightStrip
