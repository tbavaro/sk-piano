class Visualizer
  ###
  Base class for all visualizers.
  ###

  reset: () ->
    ###
    Resets all state for this visualizer.
    ###

  render: (secondsSinceLastFrame) ->
    ###
    Renders the next frame for this visualizer, the last frame having been
    rendered approximately `secondsSinceLastFrame` seconds ago.  If this is the
    first frame, `secondsSinceLastFrame` will be 0.
    ###
    throw "abstract"

module.exports = Visualizer
