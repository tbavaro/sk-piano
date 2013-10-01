SwitchingVisualizer = require("./SwitchingVisualizer")
PianoKeys = require("./PianoKeys")

TwinkleVisualizer = require("./TwinkleVisualizer")
TwinkleVisualizerAllWhite = require("./TwinkleVisualizerAllWhite")

class MasterVisualizer extends SwitchingVisualizer
  constructor: (strip, pianoKeys) ->
    visualizerFunctors = [
      () -> new TwinkleVisualizer(strip, pianoKeys)
      () -> new TwinkleVisualizerAllWhite(strip, pianoKeys)
    ]
    super(pianoKeys, visualizerFunctors)

module.exports = MasterVisualizer
