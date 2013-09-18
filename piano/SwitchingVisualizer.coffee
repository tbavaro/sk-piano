Visualizer = require("./Visualizer")
PianoKeys = require("./PianoKeys")
TestUtils = require("./TestUtils")
assert = require("assert")

# switch whenever the leftmost key is pressed
SWITCH_KEY = 0

class SwitchingVisualizer extends Visualizer
  constructor: (pianoKeys, visualizerFunctors) ->
    super
    assert(visualizerFunctors.length > 0)
    @visualizerFunctors = visualizerFunctors
    @pianoKeys = pianoKeys
    @reset()

  reset: () ->
    super
    @currentVisualizerIndex = -1
    @switchToNextVisualizer()

  switchToNextVisualizer: () ->
    @currentVisualizerIndex =
        ((@currentVisualizerIndex + 1) % @visualizerFunctors.length)
    duration = TestUtils.runTimed () =>
      @currentVisualizer = (@visualizerFunctors[@currentVisualizerIndex])()
    console.log([
      "Switching to visualizer: ", @currentVisualizer, " (", duration, "ms)"
    ].join(""))
    @currentVisualizer.reset()
    @currentVisualizer.render(0)

  render: (secondsSinceLastFrame) ->
    if SWITCH_KEY in @pianoKeys.pressedSinceLastFrame
      @switchToNextVisualizer()
    else
      @currentVisualizer.render(secondsSinceLastFrame)
    return

module.exports = SwitchingVisualizer
