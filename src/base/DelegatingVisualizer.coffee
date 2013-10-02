Visualizer = require("base/Visualizer")

class DelegatingVisualizer extends Visualizer
  constructor: () ->
    @_delegate = null
    super

  setDelegate: (visualizer) ->
    @_delegate = visualizer
    visualizer.reset()
    visualizer.render(0)
    return

  reset: () ->
    super
    @_delegate.reset() if @_delegate != null
    return

  render: (secondsSinceLastFrame) ->
    @_delegate.render(secondsSinceLastFrame) if @_delegate != null
    return

module.exports = DelegatingVisualizer
