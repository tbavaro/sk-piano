Visualizer = require("lib/Visualizer")

class CompositeVisualizer extends Visualizer
  constructor: (delegates) ->
    super
    @delegates = delegates

  reset: () ->
    super
    delegate.reset() for delegate in @delegates
    return

  render: (secondsSinceLastFrame) ->
    delegate.render(secondsSinceLastFrame) for delegate in @delegates
    return

module.exports = CompositeVisualizer
