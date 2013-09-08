Visualizer = require("./Visualizer")

class CompositeVisualizer extends Visualizer
  constructor: (visualizers) ->
    @visualizers = visualizers

  reset: () ->
    v.reset for v in @visualizers

  setPressedKeys: (pressed_keys) ->
    v.setPressedKeys(pressed_keys) for v in @visualizers

module.exports = CompositeVisualizer