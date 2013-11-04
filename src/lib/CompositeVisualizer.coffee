Visualizer = require("./Visualizer")

class CompositeVisualizer extends Visualizer
  ###
  A visualizer made up of other visualizers, all of which are rendered every
  frame.  This is mostly useful for having several different visualizers (or
  instances of the same visualizer) each responsible for rendering a different
  region.
  ###

  constructor: (visualizers) ->
    ###
    Construct a `CompositeVisualizer` made up of the given visualizers.
    ###
    super
    @visualizers = visualizers

  reset: () ->
    ###
    Resets the state for all sub-visualizers.
    ###
    super
    visualizer.reset() for visualizer in @visualizers
    return

  render: (secondsSinceLastFrame) ->
    ###
    Renders each of the sub-visualizers.
    ###
    visualizer.render(secondsSinceLastFrame) for visualizer in @visualizers
    return

module.exports = CompositeVisualizer
