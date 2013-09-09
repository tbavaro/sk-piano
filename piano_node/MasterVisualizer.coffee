SwitchingVisualizer = require("./SwitchingVisualizer")
PianoKeys = require("./PianoKeys")

TwinkleVisualizer = require("./TwinkleVisualizer")

class MasterVisualizer extends SwitchingVisualizer
  constructor: (strip, pianoKeys) ->
    visualizerFunctors = [
      () -> new TwinkleVisualizer(strip, pianoKeys)
    ]
    super(pianoKeys, visualizerFunctors)

module.exports = MasterVisualizer
