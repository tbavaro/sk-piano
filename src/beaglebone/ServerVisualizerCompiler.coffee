VisualizerCompiler = require("base/VisualizerCompiler")

# on node we need to include this, but in the browser we have to separately
# pull in the browser-compatible implementation
global.CoffeeScript = require("coffee-script")

module.exports = VisualizerCompiler
