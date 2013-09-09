SwitchingVisualizer = require("./SwitchingVisualizer")
PianoKeys = require("./PianoKeys")

TwinkleVisualizer = require("./TwinkleVisualizer")

class MasterVisualizer extends SwitchingVisualizer
  constructor: (strip, pianoKeys) ->
    visualizers = [
      new TwinkleVisualizer(strip, pianoKeys)
    ]
    super(pianoKeys, visualizers)

module.exports = MasterVisualizer
