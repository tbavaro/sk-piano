# Base class for all visualizers.
# @author tbavaro
module.exports = class Visualizer
  # Resets all state for this visualizer.
  reset: () ->

  # Renders the next frame for this visualizer.
  # @param {Float} secondsSinceLastFrame approximate number of seconds since the last frame was rendered; if this is the first frame, this will be 0.
  # @abstract
  render: (secondsSinceLastFrame) ->
    throw "abstract"
