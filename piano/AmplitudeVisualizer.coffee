Visualizer = require("./Visualizer")
PianoKeys = require("./PianoKeys")

class AmplitudeVisualizer extends Visualizer
  constructor: (pianoKeys, noteIncrease, decreaseRate, maxValue) ->
    @pianoKeys = pianoKeys
    @noteIncrease = noteIncrease
    @decreaseRate = decreaseRate
    @maxValue = maxValue
    @value = 0

  reset: () ->
    super
    @value = 0
    @renderValue(@value)
    return

  render: (secondsSinceLastFrame) ->
    # decay, but don't go below zero
    @value = Math.max(0, @value - secondsSinceLastFrame * @decreaseRate)

    # increase for pressed keys, but don't go above maxValue
    @value = Math.min(
        @maxValue,
        @value + @noteIncrease * @pianoKeys.pressedSinceLastFrame.length)

    @renderValue(@value, secondsSinceLastFrame)
    return

  renderValue: (value, secondsSinceLastFrame) -> throw "abstract"

module.exports = AmplitudeVisualizer
