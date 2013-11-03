require("base/VisualizerGlobals")

class VisualizerCompiler
  compile: (code) ->
    # wrap it in a coffeescript function to get implicit return
    wrappedCoffeeCode = [
      "return ((() ->",
      # indent every line; there's a chance this could mess up some things in
      # weird circumstances (like multi-line string literals) but that seems
      # unlikely for visualizers
      code.replace(/.+/g, "  $&"),
      ")())"
    ].join("\n")
    "return " + CoffeeScript.compile(wrappedCoffeeCode)

  instantiate: (code, strip, pianoKeys) ->
    visualizerClass = Function(@compile(code))()
    new visualizerClass(strip, pianoKeys)

module.exports = new VisualizerCompiler()
