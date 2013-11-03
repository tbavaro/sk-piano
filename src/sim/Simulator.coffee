DataStore = require("sim/DataStore")
PianoKeyboard = require("sim/PianoKeyboard")
SimulatorCodeEditor = require("sim/SimulatorCodeEditor")
SimulatorPiano = require("sim/SimulatorPiano")
ViewPort = require("sim/ViewPort")
VisualizerCompiler = require("base/VisualizerCompiler")

# after a change, if idle for this many ms then we will save the document
IDLE_SAVE_INTERVAL = 2000

KEY_R = 82
KEY_S = 83

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
#      console.log("changed")
      @isDirty = true
      if @saveChangeTimeoutId != null then clearTimeout(@saveChangeTimeoutId)
      @saveChangeTimeoutId =
          setTimeout (() => @saveDocumentIfDirty()), IDLE_SAVE_INTERVAL

    @loadedDocumentName = @dataStore.loadedDocumentName()
    if @loadedDocumentName == null
      console.log("loading default visualizers into datastore...")
      @editor.setActive(false)
      @loadDefaultVisualizers()
    else
      @loadDocument(@loadedDocumentName)

    $("#run_button").click () => @runCode()

    $(document).on "keydown", (event) =>
      specialFunc = null

      if (event.ctrlKey || event.metaKey)
        if event.shiftKey
          switch event.which
            # cmd + shift + r: RUN
            when KEY_R then specialFunc = () => @runCode()
            else null # no-op
        else
          switch event.which
            # cmd + s: SAVE
            when KEY_S then specialFunc = () => @saveDocumentIfDirty()
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
      name = @dataStore.newDocument(defaultVisualizer)
      @dataStore.setDocumentContent(name, content)
      @loadDocument(name)

  loadDocument: (name) ->
    content = @dataStore.documentContent(name)
    $("#title_label").text(name)
    @editor.setContent(content)
    @editor.setActive(true)
    @dataStore.setLoadedDocumentName(name)
    @loadedDocumentName = name
    @isDirty = false

  saveDocument: () ->
    if @loadedDocumentName == null then throw "no document loaded"
    @dataStore.setDocumentContent(@loadedDocumentName, @editor.content())
    @isDirty = false
    console.log("Saved document #{@loadedDocumentName}")

  saveDocumentIfDirty: () ->
    if @loadedDocumentName != null && @isDirty then @saveDocument()

  runCode: () ->
    # save before running since it's possible the code will crash the app
    @saveDocumentIfDirty()

    code = @editor.content()

    visualizer =
        VisualizerCompiler.instantiate(code, @piano.strip, @piano.pianoKeys)

    @piano.setVisualizer(visualizer)

module.exports = Simulator
