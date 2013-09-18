SimulatorCodeEditor = require("sim/SimulatorCodeEditor")

class Simulator
  constructor: () ->
    @editor = new SimulatorCodeEditor(document.getElementById("editor"))

module.exports = Simulator
