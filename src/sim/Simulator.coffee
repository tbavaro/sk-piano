PianoKeyboard = require("sim/PianoKeyboard")
SimulatorCodeEditor = require("sim/SimulatorCodeEditor")
SimulatorPiano = require("sim/SimulatorPiano")
ViewPort = require("sim/ViewPort")

class MyPianoKeyboard extends PianoKeyboard
  constructor: (piano) ->
    super(document.getElementById("piano_keys"))
    @piano = piano

  onNoteDown: (note) ->
    super(note)
    @piano.pianoKeys.keyStates[note] = true

  onNoteUp: (note) ->
    super(note)
    @piano.pianoKeys.keyStates[note] = false

class Simulator
  constructor: () ->
    @piano = new SimulatorPiano()
    @editor = new SimulatorCodeEditor(document.getElementById("editor"))
    @viewPort = new ViewPort(document.getElementById("piano_viewport"), @piano.strip)
    @pianoKeyboard = new MyPianoKeyboard(@piano)

    $.ajax({
      url: "visualizers/TwinkleVisualizer.coffee",
      async: false
    }).done (content) =>
      @editor.codeMirror.setValue(content)

module.exports = Simulator
