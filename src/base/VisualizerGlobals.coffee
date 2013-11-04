INCLUDES =
  AmplitudeVisualizer: require("lib/AmplitudeVisualizer")
  Colors: require("lib/Colors")
  CompositeVisualizer: require("lib/CompositeVisualizer")
  LightStrip: require("lib/LightStrip")
  LogicalLightStrip: require("lib/LogicalLightStrip")
  PianoKeys: require("lib/PianoKeys")
  PianoLocations: require("lib/PianoLocations")
  Visualizer: require("lib/Visualizer")

if (typeof global) == "undefined" && (typeof window) != "undefined"
  `global = window`

for name, value of INCLUDES
  console.log("adding #{name}")
  global[name] = value
