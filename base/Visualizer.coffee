class Visualizer
  reset: () ->

  render: (secondsSinceLastFrame) -> throw "abstract"

module.exports = Visualizer
