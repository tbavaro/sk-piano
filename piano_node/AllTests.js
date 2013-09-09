require("coffee-script")

// TODO build this programmatically by looking at the filesystem
testClassNames = [
  "FrameBufferLightStripTests",
  "PianoKeysTests"
]

testClassNames.forEach(function(className) {
  exports[className] = require("./" + className);
});
