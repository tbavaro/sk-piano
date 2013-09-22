SimulatorCodeEditor = require("sim/SimulatorCodeEditor")
ViewPort = require("sim/ViewPort")

class Simulator
  constructor: () ->
    @editor = new SimulatorCodeEditor(document.getElementById("editor"))
    @viewPort = new ViewPort(document.getElementById("piano_viewport"))

module.exports = Simulator

