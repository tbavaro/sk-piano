NUM_KEYS = 88

NOTE_NAMES = [
  "a", "A", "b", "c", "C", "d", "D", "e", "f", "F", "g", "G"
]

class PianoKeys
  @NUM_KEYS: NUM_KEYS

  constructor: () ->
    # invariant: @pressedKeys contains exactly the indexes of
    # @keys which are true, and @_pressedKeys2 contains exactly
    # the indexes of @_keys2 which are true
    @keys = (false for _ in [0...NUM_KEYS] by 1)
    @_keys2 = (false for _ in [0...NUM_KEYS] by 1)
    @pressedKeys = []
    @_pressedKeys2 = []
    @pressedSinceLastFrame = []
    @releasedSinceLastFrame = []

  setPressedKeys: (pressedKeys) ->
    # reuse old @_keys2 array by blanking out the values where
    # it is true and then setting the pressedKeys indexes to true
    newKeys = @_keys2
    newKeys[key] = false for key in @_pressedKeys2
    newKeys[key] = true for key in pressedKeys

    # update pressed/released-since-last-frame arrays
    oldKeys = @keys
    @pressedSinceLastFrame = []
    @releasedSinceLastFrame = []
    for key in [0...NUM_KEYS] by 1
      if newKeys[key] != oldKeys[key]
        array =
          (if newKeys[key] then @pressedSinceLastFrame
          else @releasedSinceLastFrame)
        array.push(key)

    @_pressedKeys2 = @pressedKeys
    @_keys2 = oldKeys
    @pressedKeys = pressedKeys
    @keys = newKeys

    return

  note: (key) ->
    NOTE_NAMES[key % NOTE_NAMES.length]

  octave: (key) ->
    Math.floor(key / NOTE_NAMES.length)

  keyName: (key) ->
    @note(key) + @octave(key)

  pressedKeyNames: () ->
    @keyName(key) for key in @pressedKeys

module.exports = PianoKeys
