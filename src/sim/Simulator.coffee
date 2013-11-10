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

class Dropdown
  @ACTIVE_CLASS: "active"
  @BACKSTOP_ID: "dropdown_backstop"
  @CONTENTS_ID: "dropdown_contents"

  constructor: (buttonElement) ->
    @buttonElement = buttonElement
    @backstopElement = $("##{Dropdown.BACKSTOP_ID}")
    @contentsElement = $("##{Dropdown.CONTENTS_ID}")
    @buttonElement.click @show.bind(this)
    @backstopElement.click @hide.bind(this)

  isActive: () ->
    @buttonElement.hasClass(Dropdown.ACTIVE_CLASS)

  show: () -> @setIsActive(true)
  hide: () -> @setIsActive(false)

  setIsActive: (value) ->
    if (value)
      buttonPosition = @buttonElement.offset()
      @contentsElement.css({
        left: buttonPosition.left,
        top: buttonPosition.top + @buttonElement.outerHeight(),
        width: @buttonElement.outerWidth()
      })
      @fillContents(@contentsElement)

    for element in [@buttonElement, @contentsElement, @backstopElement]
      if (value)
        element.addClass(Dropdown.ACTIVE_CLASS)
      else
        element.removeClass(Dropdown.ACTIVE_CLASS)

    if (!value)
      @contentsElement.empty()

  fillContents: (element) -> return # defaults to empty

class TitleBarDropdown extends Dropdown
  constructor: (buttonElement, datastore, actions) ->
    super(buttonElement)
    @datastore = datastore
    @actions = actions

  show: () ->
    @actions.saveDocumentIfDirty()
    super

  createDiv: () -> $(document.createElement("div"))

  createEntryElement: (name, isLoaded) ->
    @createDiv()
        .addClass("button enabled document" + (if isLoaded then " active" else ""))
        .text(name)
        .click () =>
          @actions.loadDocument(name)
          @hide()

  createSeparatorElement: () ->
    @createDiv().addClass("separator")

  createActionElement: (name, isEnabled, func) ->
    @createDiv()
        .addClass("button" + (if isEnabled then " enabled" else ""))
        .text(name)
        .click () =>
          func()
          @hide()

  fillContents: (element) ->
    contentsElement =
        $(document.createElement("div")).addClass("title-dropdown")
    element.append(contentsElement)

    loadedDocumentName = @datastore.loadedDocumentName()
    documentNames = @datastore.documentNames()
    for name in documentNames
      isLoaded = (name == loadedDocumentName)
      contentsElement.append(@createEntryElement(name, isLoaded))
    contentsElement.append([
      @createSeparatorElement()
      @createActionElement "Delete...", (documentNames.length > 1), () => @actions.deleteDocument()
      @createActionElement "Duplicate...", true, () => @actions.duplicateDocument()
      @createActionElement "Rename...", true, () => @actions.renameDocument()
#      @createSeparatorElement()
#      @createActionElement("Add to piano...", false, (() -> return))
    ])

class Actions
  constructor: (simulator) ->
    @simulator = simulator
    @datastore = simulator.dataStore

  saveDocumentIfDirty: () ->
    @simulator.saveDocumentIfDirty()

  loadedDocumentName: () ->
    @datastore.loadedDocumentName()

  duplicateDocument: () ->
    loadedDocumentName = @loadedDocumentName()
    newName = @datastore.defaultDuplicateDocumentName(loadedDocumentName)
    newName = window.prompt("New document name", newName)
    if newName != null
      newName = @datastore.duplicateDocument(loadedDocumentName, newName)
      @loadDocument(newName)

  renameDocument: () ->
    loadedDocumentName = @loadedDocumentName()
    newName = window.prompt("New document name", loadedDocumentName)
    if newName != null
      newName = @datastore.renameDocument(loadedDocumentName, newName)
      @loadDocument(newName)

  deleteDocument: () ->
    ok = window.confirm([
      "You are about to delete the document."
      "This cannot be undone."
    ].join(" "))
    if ok
      @datastore.deleteDocument(@loadedDocumentName())
      @loadDocument(@datastore.documentNames()[0])

  loadDocument: (name) ->
    @saveDocumentIfDirty()
    @simulator.loadDocument(name)

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

module.exports = class Simulator
  constructor: () ->
    @piano = new SimulatorPiano()
    @dataStore = new DataStore()
    @editor = new SimulatorCodeEditor(document.getElementById("editor"))
    @viewPort = new ViewPort(document.getElementById("piano_viewport"), @piano.strip)
    @pianoKeyboard = new MyPianoKeyboard(@piano)
    @actions = new Actions(this)

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

    @titleBarDropdown =
      new TitleBarDropdown($("#title_bar"), @dataStore, @actions)

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

  toggleTitleBar: () ->
