class DebugVisualizer extends Visualizer
  constructor: (strip, pianoKeys) ->
    super
    @strip = strip
    @pianoKeys = pianoKeys

  render: (secondsSinceLastFrame) ->
    valuesToPrint = []

    valuesToPrint.push("+#{PianoKeys.keyName(key)}") for key in @pianoKeys.pressedSinceLastFrame
    valuesToPrint.push("-#{PianoKeys.keyName(key)}") for key in @pianoKeys.releasedSinceLastFrame

    if valuesToPrint.length > 0
      console.log(valuesToPrint.join(" "))
