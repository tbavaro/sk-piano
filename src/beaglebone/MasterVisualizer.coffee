SwitchingVisualizer = require("base/SwitchingVisualizer")
PianoKeys = require("lib/PianoKeys")
VisualizerCompiler = require("beaglebone/ServerVisualizerCompiler")
ServerVisualizerLibrary = require("base/ServerVisualizerLibrary")

module.exports = class MasterVisualizer extends SwitchingVisualizer
  constructor: (strip, pianoKeys) ->
    @library = ServerVisualizerLibrary.activeVisualizers()
    @strip = strip
    @pianoKeys = pianoKeys
    super(pianoKeys, @createVisualizerFunctors())
    @library.watch () => @reloadVisualizerFunctors()

  createVisualizerFunctors: () ->
    visualizers = @library.list()
    console.log "Loaded visualizers: #{visualizers}"
    @makeVisualizerFunctor(visualizer) for visualizer in visualizers

  makeVisualizerFunctor: (visualizer) ->
    () =>
      code = @library.read(visualizer)
      v = VisualizerCompiler.instantiate(code, @strip, @pianoKeys)
      v.toString = () => visualizer
      v

  reloadVisualizerFunctors: () ->
    @setVisualizerFunctors(@createVisualizerFunctors())
