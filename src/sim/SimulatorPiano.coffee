Piano = require("base/Piano")
FrameBufferLightStrip = require("base/FrameBufferLightStrip")
LedLocations = require("sim/LedLocations")
DelegatingVisualizer = require("base/DelegatingVisualizer")
PianoKeys = require("base/PianoKeys")

convertColor = (pianoColor) ->
  new THREE.Color(pianoColor * 2).convertLinearToGamma()

class SimulatorLightStrip extends FrameBufferLightStrip
  constructor: (numPixels) ->
    super(numPixels)
    @cachedColors = null
    @reset()

  reset: () ->
    super
    @cachedColors = null

  display: () ->
    @cachedColors = null

  colors: () ->
    @cachedColors = (convertColor(c) for c in @pixels) if @cachedColors == null
    @cachedColors

class SimulatorPianoKeys extends PianoKeys
  constructor: () ->
    super
    @keyStates = (false for _ in [0...88] by 1)

  scan: () ->
    pressedKeys = (i for i in [0...88] by 1 when @keyStates[i])
    @setPressedKeys(pressedKeys)

class SimulatorPiano extends Piano
  constructor: () ->
    strip = new SimulatorLightStrip(LedLocations.length)
    pianoKeys = new SimulatorPianoKeys()
    visualizer = new DelegatingVisualizer()
    super(strip, pianoKeys, visualizer)

  setVisualizer: (visualizer) ->
    @visualizer.setDelegate(visualizer)

module.exports = SimulatorPiano
