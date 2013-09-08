class Visualizer
  reset: () -> setPressedKeys([])
  setPressedKeys: (pressed_keys) -> throw "abstract"

module.exports = Visualizer
