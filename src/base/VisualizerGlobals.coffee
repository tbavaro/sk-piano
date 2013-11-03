INCLUDES =
  AmplitudeVisualizer: require("base/AmplitudeVisualizer")
  Colors: require("base/Colors")
  CompositeVisualizer: require("base/CompositeVisualizer")
  LogicalLightStrip: require("base/LogicalLightStrip")
  PianoKeys: require("base/PianoKeys")
  PianoLocations: require("base/PianoLocations")
  Visualizer: require("base/Visualizer")

if (typeof global) == "undefined" && (typeof window) != "undefined"
  `global = window`

for name, value of INCLUDES
  console.log("adding #{name}")
  global[name] = value
