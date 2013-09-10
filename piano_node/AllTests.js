require("coffee-script")

// TODO build this programmatically by looking at the filesystem
testClassNames = [
  "PhysicalLightStripTests",
  "PhysicalPianoKeysTests",
  "PianoKeysTests"
]

testClassNames.forEach(function(className) {
  exports[className] = require("./" + className);
});
