SimulatorCodeEditor = require("sim/SimulatorCodeEditor")
SimulatorPiano = require("sim/SimulatorPiano")
ViewPort = require("sim/ViewPort")

class Simulator
  constructor: () ->
    @editor = new SimulatorCodeEditor(document.getElementById("editor"))
    @piano = new SimulatorPiano()
    @viewPort = new ViewPort(document.getElementById("piano_viewport"), @piano.strip)

    $.ajax({
      url: "visualizers/TwinkleVisualizer.coffee",
      async: false
    }).done (content) =>
      @editor.codeMirror.setValue(content)

module.exports = Simulator
