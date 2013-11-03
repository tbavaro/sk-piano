SwitchingVisualizer = require("base/SwitchingVisualizer")
PianoKeys = require("base/PianoKeys")
ServerVisualizerCompiler = require("beaglebone/ServerVisualizerCompiler")
TwinkleVisualizer = require("visualizers/TwinkleVisualizer")

class MasterVisualizer extends SwitchingVisualizer
  constructor: (strip, pianoKeys) ->
    visualizerFunctors = [
      () -> new TwinkleVisualizer(strip, pianoKeys)
    ]
    super(pianoKeys, visualizerFunctors)

module.exports = MasterVisualizer
