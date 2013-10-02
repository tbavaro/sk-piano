PianoKeyboard = require("sim/PianoKeyboard")
SimulatorCodeEditor = require("sim/SimulatorCodeEditor")
SimulatorPiano = require("sim/SimulatorPiano")
TwinkleVisualizer = require("visualizers/TwinkleVisualizer")
ViewPort = require("sim/ViewPort")

KEY_R = 82
KEY_S = 83

compileModule = (code) ->
  wrappedCode = [
    "var module = { exports: null };",
    CoffeeScript.compile(code),
    "return module.exports;",
  ].join("\n")
  Function(wrappedCode)()

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

    $("#run_button").click () => @runCode()

    $(document).on "keydown", (event) =>
      specialFunc = null

      # cmd-R to run
      if (event.which == KEY_R && (event.ctrlKey || event.metaKey))
        specialFunc = () => @runCode()

      if specialFunc != null
        event.preventDefault()
        specialFunc()
        false
      else
        true

  runCode: () ->
    code = @editor.codeMirror.getValue()
    @piano.setVisualizerClass(compileModule(code))

module.exports = Simulator
