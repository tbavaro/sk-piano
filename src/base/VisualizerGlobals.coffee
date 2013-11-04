INCLUDES =
  AmplitudeVisualizer: require("lib/AmplitudeVisualizer")
  Colors: require("lib/Colors")
  CompositeVisualizer: require("lib/CompositeVisualizer")
  LightStrip: require("lib/LightStrip")
  LogicalLightStrip: require("lib/LogicalLightStrip")
  PianoKeys: require("lib/PianoKeys")
  LEDLocations: require("lib/LEDLocations")
  LEDRanges: require("lib/LEDRanges")
  Visualizer: require("lib/Visualizer")

if (typeof global) == "undefined" && (typeof window) != "undefined"
  `global = window`

for name, value of INCLUDES
  console.log("adding #{name}")
  global[name] = value
