# @private
NOTE_NAMES = [
  "a", "A", "b", "c", "C", "d", "D", "e", "f", "F", "g", "G"
]

# Provides access to read the piano key states.
# @author tbavaro
module.exports = class PianoKeys
  # The total number of keys on the piano
  @NUM_KEYS: 88

  # @property {Array<Boolean>} `NUM_KEYS`-length array of booleans representing whether or not the key at each position is down
  keys: null

  # @property {Array<Key>} the keys which are currently pressed
  pressedKeys: null

  # @property {Array<Key>} the keys which have been pressed since the last frame
  pressedSinceLastFrame: null

  # @property {Array<Key>} the keys which have been released since the last frame
  releasedSinceLastFrame: null

  # @private should only be called by the framework
  constructor: () ->
    # invariant: @pressedKeys contains exactly the indexes of
    # @keys which are true, and @_pressedKeys2 contains exactly
    # the indexes of @_keys2 which are true
    @keys = (false for _ in [0...PianoKeys.NUM_KEYS] by 1)
    @_keys2 = (false for _ in [0...PianoKeys.NUM_KEYS] by 1)
    @pressedKeys = []
    @_pressedKeys2 = []
    @pressedSinceLastFrame = []
    @releasedSinceLastFrame = []

  # @private should only be called by the framework
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
    for key in [0...PianoKeys.NUM_KEYS] by 1
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

  # Gets the note number for the given key.
  # @param {Key} key the key
  # @return {Integer} the note number, from 0 to 11
  @note: (key) ->
    key % NOTE_NAMES.length

  # Gets the octave number for the given key.
  # @param {Key} key the key
  # @return {Integer} the octave number, from 1 to 8
  @octave: (key) ->
    Math.floor(key / NOTE_NAMES.length) + 1

  # @private
  @keyName: (key) ->
    NOTE_NAMES[@note(key)] + @octave(key)

  # Gets the names of the currently-pressed keys; useful for debugging.  Note
  # names are always 2 characters, the note letter (uppercase for sharps)
  # followed by the octave.  For example, "c4" is middle C, "C4" is the adjacent
  # C#.
  # @return {Array<String>}
  pressedKeyNames: () ->
    @keyName(key) for key in @pressedKeys
