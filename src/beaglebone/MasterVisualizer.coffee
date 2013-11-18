SwitchingVisualizer = require("base/SwitchingVisualizer")
PianoKeys = require("lib/PianoKeys")
VisualizerCompiler = require("beaglebone/ServerVisualizerCompiler")
VisualizerLibrary = require("base/VisualizerLibrary")

module.exports = class MasterVisualizer extends SwitchingVisualizer
  constructor: (strip, pianoKeys) ->
    @library = VisualizerLibrary.activeVisualizers()
    @strip = strip
    @pianoKeys = pianoKeys
    super(pianoKeys, @createVisualizerFunctors())
    @library.watch () => @reloadVisualizerFunctors()

  createVisualizerFunctors: () ->
    visualizers = @library.list()
    console.log "Loaded visualizers: #{visualizers}"
    (() =>
      code = @library.read(visualizer)
      v = VisualizerCompiler.instantiate(code, @strip, @pianoKeys)
      v.toString = () => visualizer
      v
    ) for visualizer in visualizers

  reloadVisualizerFunctors: () ->
    @setVisualizerFunctors(@createVisualizerFunctors())
