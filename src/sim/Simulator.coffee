DataStore = require("sim/DataStore")
PianoKeyboard = require("sim/PianoKeyboard")
SimulatorCodeEditor = require("sim/SimulatorCodeEditor")
SimulatorPiano = require("sim/SimulatorPiano")
TwinkleVisualizer = require("visualizers/TwinkleVisualizer")
ViewPort = require("sim/ViewPort")

# after a change, if idle for this many ms then we will save the document
IDLE_SAVE_INTERVAL = 2000

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
    @dataStore = new DataStore()
    @editor = new SimulatorCodeEditor(document.getElementById("editor"))
    @viewPort = new ViewPort(document.getElementById("piano_viewport"), @piano.strip)
    @pianoKeyboard = new MyPianoKeyboard(@piano)

    @isDirty = false
    @saveChangeTimeoutId = null
    @editor.addOnChangeHandler () =>
      console.log("changed")
      @isDirty = true
      if @saveChangeTimeoutId != null then clearTimeout(@saveChangeTimeoutId)
      @saveChangeTimeoutId =
          setTimeout (() => @saveDocumentIfDirty()), IDLE_SAVE_INTERVAL

    @loadedDocumentId = @dataStore.loadedDocumentId()
    if @loadedDocumentId == null
      @editor.setActive(false)
      @loadDefaultVisualizers()
    else
      @loadDocument(@loadedDocumentId)

    $("#run_button").click () => @runCode()

    $(document).on "keydown", (event) =>
      specialFunc = null

      if (event.ctrlKey || event.metaKey) && event.shiftKey
        switch event.which
          when KEY_R then specialFunc = () => @runCode()
          else null # no-op

      if specialFunc != null
        event.preventDefault()
        specialFunc()
        false
      else
        true

  loadDefaultVisualizers: () ->
    defaultVisualizer = "TwinkleVisualizer"
    $.ajax({
      url: "visualizers/#{defaultVisualizer}.coffee",
      async: false
    }).done (content) =>
      id = @dataStore.newDocument()
      @dataStore.setDocumentName(id, defaultVisualizer)
      @dataStore.setDocumentContent(id, content)
      @loadDocument(id)

  loadDocument: (id) ->
    name = @dataStore.documentName(id)
    content = @dataStore.documentContent(id)
    $("#title_label").text(name)
    @editor.setContent(content)
    @editor.setActive(true)
    @dataStore.setLoadedDocumentId(id)
    @loadedDocumentId = id
    @isDirty = false

  saveDocument: () ->
    if @loadedDocumentId == null then throw "no document loaded"
    @dataStore.setDocumentContent(@loadedDocumentId, @editor.content())
    @isDirty = false
    console.log("Saved document #{@loadedDocumentId}")

  saveDocumentIfDirty: () ->
    if @loadedDocumentId != null && @isDirty then @saveDocument()

  runCode: () ->
    # save before running since it's possible the code will crash the app
    @saveDocumentIfDirty()

    code = @editor.content()
    @piano.setVisualizerClass(compileModule(code))

module.exports = Simulator
