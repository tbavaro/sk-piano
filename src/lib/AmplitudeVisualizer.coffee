Visualizer = require("./Visualizer")
PianoKeys = require("./PianoKeys")

# A {Visualizer} abstraction for visualizers that make use of a single
# "amplitude" value representing the rate at which notes have been played
# recently.  Note that these visualizers can still access the `pianoKeys`
# property to also get full piano key input.
# @author tbavaro
module.exports = class AmplitudeVisualizer extends Visualizer
  # @property {PianoKeys} the {PianoKeys} object that was passed into the constructor
  pianoKeys: null

  # @param {PianoKeys} pianoKeys
  # @param {Float} noteIncrease amount to increase the value for every new key pressed since last frame
  # @param {Float} decreaseRate the rate at which the value decreases every second
  # @param {Float} maxValue cap on the maximum value that can be achieved
  # @return {AmplitudeVisualizer}
  constructor: (pianoKeys, noteIncrease, decreaseRate, maxValue) ->
    @pianoKeys = pianoKeys
    @noteIncrease = noteIncrease
    @decreaseRate = decreaseRate
    @maxValue = maxValue
    @value = 0

  # Resets all state for this visualizer.
  reset: () ->
    super
    @value = 0
    @renderValue(@value)
    return

  # Renders the next frame for this visualizer.
  # @param {Float} secondsSinceLastFrame approximate number of seconds since the last frame was rendered; if this is the first frame, this will be 0.
  # @note {Visualizer}s derived from {AmplitudeVisualizer} should override the `renderValue` method instead.
  render: (secondsSinceLastFrame) ->
    # decay, but don't go below zero
    @value = Math.max(0, @value - secondsSinceLastFrame * @decreaseRate)

    # increase for pressed keys, but don't go above maxValue
    @value = Math.min(
        @maxValue,
        @value + @noteIncrease * @pianoKeys.pressedSinceLastFrame.length)

    @renderValue(@value, secondsSinceLastFrame)
    return

  # Renders the next frame for this visualizer.
  # @param {Float} value the current amplitude value
  # @param {Float} secondsSinceLastFrame approximate number of seconds since the last frame was rendered; if this is the first frame, this will be 0.
  # @abstract
  renderValue: (value, secondsSinceLastFrame) -> throw "abstract"
