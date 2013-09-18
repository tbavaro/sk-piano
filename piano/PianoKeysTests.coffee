PianoKeys = require("./PianoKeys")
TestUtils = require("./TestUtils")
assert = require("assert")

# NB: assumes pianoKeys.pressedKeys is sorted
assertKeysAndPressedKeysConsistent = (pianoKeys) ->
  foundKeys = (key for isDown, key in pianoKeys.keys when isDown)
  assert.deepEqual(pianoKeys.pressedKeys, foundKeys)

generateRandomPressedKeys = () ->
  (key for key in [0...PianoKeys.NUM_KEYS] by 1 when Math.random() < 0.1)

exports.testCorrectness = (test) ->
  pianoKeys = new PianoKeys()

  # test starting state
  assert.deepEqual([], pianoKeys.pressedKeys)
  assert.deepEqual([], pianoKeys.pressedSinceLastFrame)
  assert.deepEqual([], pianoKeys.releasedSinceLastFrame)
  assertKeysAndPressedKeysConsistent(pianoKeys)

  pianoKeys.setPressedKeys([1])
  assert.deepEqual([1], pianoKeys.pressedKeys)
  assert.deepEqual([1], pianoKeys.pressedSinceLastFrame)
  assert.deepEqual([], pianoKeys.releasedSinceLastFrame)
  assertKeysAndPressedKeysConsistent(pianoKeys)

  pianoKeys.setPressedKeys([1, 2])
  assert.deepEqual([1, 2], pianoKeys.pressedKeys)
  assert.deepEqual([2], pianoKeys.pressedSinceLastFrame)
  assert.deepEqual([], pianoKeys.releasedSinceLastFrame)
  assertKeysAndPressedKeysConsistent(pianoKeys)

  pianoKeys.setPressedKeys([2, 3])
  assert.deepEqual([2, 3], pianoKeys.pressedKeys)
  assert.deepEqual([3], pianoKeys.pressedSinceLastFrame)
  assert.deepEqual([1], pianoKeys.releasedSinceLastFrame)
  assertKeysAndPressedKeysConsistent(pianoKeys)

  pianoKeys.setPressedKeys([1, 4])
  assert.deepEqual([1, 4], pianoKeys.pressedKeys)
  assert.deepEqual([1, 4], pianoKeys.pressedSinceLastFrame)
  assert.deepEqual([2, 3], pianoKeys.releasedSinceLastFrame)
  assertKeysAndPressedKeysConsistent(pianoKeys)

  test.done()

exports.testPerformance = (test) ->
  NUM_PASSES = 1000
  MAX_AVERAGE_PASS_MS = 0.1
  MAX_DURATION = NUM_PASSES * MAX_AVERAGE_PASS_MS

  pianoKeys = new PianoKeys()

  # generate these ahead of time
  passes = (generateRandomPressedKeys() for pass in [0...NUM_PASSES] by 1)

  TestUtils.assertMaxRunTime MAX_DURATION, () ->
    pianoKeys.setPressedKeys(pressedKeys) for pressedKeys in passes

  assertKeysAndPressedKeysConsistent(pianoKeys)

  test.done()
