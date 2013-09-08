KeyUpDownVisualizer = require("./KeyUpDownVisualizer")

class AmplitudeVisualizer extends KeyUpDownVisualizer
  constructor: (strip, note_increase, decrease_rate, max_value) ->
    super
    @strip = strip
    @note_increase = note_increase
    @decrease_rate = decrease_rate
    @max_value = max_value
    @prev_frame_time = 0
    @value = 0

  onKeyDown: (key) ->
    @value = Math.min(@value + @note_increase, @max_value)
    return

  onPassFinished: () ->
    super
    now = Date.now()
    if @prev_frame_time != 0
      secs = (now - @prev_frame_time) / 1000.0
      @value = Math.max(0, @value - secs * @decrease_rate)
    @prev_frame_time = now
    this.renderValue(@value)
    return

  renderValue: (value) ->
    throw "abstract"

  reset: () ->
    @prev_frame_time = 0
    @value = 0
    this.renderValue(@value)
    return

module.exports = AmplitudeVisualizer