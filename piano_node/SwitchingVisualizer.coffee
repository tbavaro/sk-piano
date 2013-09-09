Visualizer = require("./Visualizer")
PianoKeys = require("./PianoKeys")
assert = require("assert")

# switch whenever the leftmost key is pressed
SWITCH_KEY = 0

# TODO maybe make the visualizers on-the fly (e.g. w/ functors)
# so we can GC the ones we're not using?

class SwitchingVisualizer extends Visualizer
  constructor: (pianoKeys, visualizers) ->
    super
    assert(visualizers.length > 0)
    @visualizers = visualizers
    @pianoKeys = pianoKeys
    @reset()

  reset: () ->
    super
    @currentVisualizerIndex = -1
    @switchToNextVisualizer()

  switchToNextVisualizer: () ->
    @currentVisualizerIndex =
        ((@currentVisualizerIndex + 1) % @visualizers.length)
    @currentVisualizer = @visualizers[@currentVisualizerIndex]
    console.log("Switching to visualizer: " + @currentVisualizer)
    @currentVisualizer.reset()
    @currentVisualizer.render(0)

  render: (secondsSinceLastFrame) ->
    if SWITCH_KEY in @pianoKeys.pressedSinceLastFrame
      @switchToNextVisualizer()
    else
      @currentVisualizer.render(secondsSinceLastFrame)
    return

module.exports = SwitchingVisualizer
