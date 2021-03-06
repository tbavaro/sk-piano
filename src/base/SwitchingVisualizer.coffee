DelegatingVisualizer = require("base/DelegatingVisualizer")
PianoKeys = require("lib/PianoKeys")
TestUtils = require("test/TestUtils")
assert = require("assert")

# switch whenever the leftmost key is pressed
SWITCH_KEY = 0

class SwitchingVisualizer extends DelegatingVisualizer
  constructor: (strip, pianoKeys, visualizerFunctors) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys
    @setVisualizerFunctors(visualizerFunctors)

  setVisualizerFunctors: (visualizerFunctors) ->
    assert(visualizerFunctors.length > 0)
    @visualizerFunctors = visualizerFunctors
    @reset()

  reset: () ->
    super
    @currentVisualizerIndex = -1
    @switchToNextVisualizer()
    return

  switchToNextVisualizer: () ->
    @currentVisualizerIndex =
        ((@currentVisualizerIndex + 1) % @visualizerFunctors.length)
    visualizer = null
    duration = TestUtils.runTimed () =>
      visualizer = (@visualizerFunctors[@currentVisualizerIndex])()
    console.log([
      "Switching to visualizer: ", visualizer, " (", duration, "ms)"
    ].join(""))
    @setDelegate(visualizer)
    @strip.reset()
    return

  render: (secondsSinceLastFrame) ->
    @switchToNextVisualizer() if SWITCH_KEY in @pianoKeys.pressedSinceLastFrame
    super(secondsSinceLastFrame)

module.exports = SwitchingVisualizer
