Visualizer = require("./Visualizer")
Colors = require("./Colors")

NUM_KEYS = 88

class KeyUpDownVisualizer extends Visualizer
  constructor: () ->
    @keys = Colors.BLACK for _ in [0...NUM_KEYS]
    @keys2 = Colors.BLACK for _ in [0...NUM_KEYS]

  setPressedKeys: (pressed_keys) ->
    new_keys = @keys2
    new_keys[i] = Colors.BLACK for i in [0...NUM_KEYS]
    new_keys[key] = true for key in pressed_keys
    (if (new_keys[key] != @keys[key]) then \
      (if new_keys[key] then this.onKeyDown(key) else this.onKeyUp(key))) \
        for key in [0...NUM_KEYS]
    @keys2 = @keys
    @keys = new_keys
    this.onPassFinished()
    return

  onKeyDown: (key) -> return

  onKeyUp: (key) -> return

  onPassFinished: (key) -> return

module.exports = KeyUpDownVisualizer