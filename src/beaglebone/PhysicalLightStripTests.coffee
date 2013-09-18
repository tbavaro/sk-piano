PhysicalLightStrip = require("./PhysicalLightStrip")
Colors = require("base/Colors")
TestUtils = require("test/TestUtils")
assert = require("assert")

exports.testCorrectness = (test) ->
  strip = new PhysicalLightStrip(null, 3)

  assert.equal(3, strip.numPixels)

  TestUtils.assertBufferEquals(
    [0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00],
    strip._updateBuffer())

  assert.equal(Colors.BLACK, strip.getPixel(0))
  assert.equal(Colors.BLACK, strip.getPixel(1))
  assert.equal(Colors.BLACK, strip.getPixel(2))

  strip.setPixel(0, 0x010203)
  strip.setPixel(1, 0x040506)
  strip.setPixel(2, 0x070809)

  assert.equal(0x010203, strip.getPixel(0))
  assert.equal(0x040506, strip.getPixel(1))
  assert.equal(0x070809, strip.getPixel(2))

  TestUtils.assertBufferEquals(
    [0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x00],
    strip._updateBuffer())

  strip.setPixel(2, 0x0d0e0f)
  strip.setPixel(1, 0x0a0b0c)
  strip.setPixel(0, 0x070809)

  assert.equal(0x070809, strip.getPixel(0))
  assert.equal(0x0a0b0c, strip.getPixel(1))
  assert.equal(0x0d0e0f, strip.getPixel(2))

  TestUtils.assertBufferEquals(
    [0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x00],
    strip._updateBuffer())

  strip.reset()

  assert.equal(Colors.BLACK, strip.getPixel(0))
  assert.equal(Colors.BLACK, strip.getPixel(1))
  assert.equal(Colors.BLACK, strip.getPixel(2))

  TestUtils.assertBufferEquals(
    [0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00],
    strip._updateBuffer())

  test.done()

exports.testPerformance = (test) ->
  NUM_PIXELS = 1000
  NUM_FRAMES = 60
  MAX_DURATION = 100
  strip = new PhysicalLightStrip(null, NUM_PIXELS)

  TestUtils.assertMaxRunTime MAX_DURATION, () ->
    for _ in [0...NUM_FRAMES] by 1
      for pixel in [0...NUM_PIXELS] by 1
        strip.setPixel(pixel, pixel)
      strip._updateBuffer()
    return

  test.done()
