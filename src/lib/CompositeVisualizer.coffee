Visualizer = require("./Visualizer")

# A visualizer made up of other visualizers, all of which are rendered every
# frame.  This is mostly useful for having several different visualizers (or
# instances of the same visualizer) each responsible for rendering a different
# region of the piano.
# @author tbavaro
module.exports = class CompositeVisualizer extends Visualizer
  # Constructs a {CompositeVisualizer} made up of the given {Visualizer}s.
  # @param {Array<Visualizer>} visualizers
  # @return {CompositeVisualizer}
  constructor: (visualizers) ->
    super
    @visualizers = visualizers

  # Resets the state for all sub-visualizers.
  reset: () ->
    super
    visualizer.reset() for visualizer in @visualizers
    return

  # Renders the next frame for all sub-visualizers.
  # @param {Float} secondsSinceLastFrame
  render: (secondsSinceLastFrame) ->
    ###
    Renders each of the sub-visualizers.
    ###
    visualizer.render(secondsSinceLastFrame) for visualizer in @visualizers
    return
