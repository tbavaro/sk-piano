InMemoryVisualizerLibrary = require("sim/InMemoryVisualizerLibrary")
LocalStorageVisualizerLibrary = require("sim/LocalStorageVisualizerLibrary")
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
  constructor: (buttonElement, simulator, actions) ->
    super(buttonElement)
    @simulator = simulator
    @datastore = simulator.dataStore
    @actions = actions

  show: () ->
    @actions.saveDocumentIfDirty()
    super

  createDiv: () -> $(document.createElement("div"))

  createEntryElement: (library, name, isLoaded) ->
    @createDiv()
        .addClass("button enabled document" + (if isLoaded then " active" else ""))
        .text(name)
        .click () =>
          @actions.loadDocument(library, name)
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

    loadedDocumentName = @simulator.loadedDocumentName
    loadedDocumentLibrary = @simulator.loadedDocumentLibrary

    libraries = [
      @simulator.activeLibrary,
      @simulator.tutorialLibrary,
      @simulator.dataStore
    ]

    for library in libraries
      for name in library.list()
        isLoaded = (name == loadedDocumentName && library == loadedDocumentLibrary)
        contentsElement.append(@createEntryElement(library, name, isLoaded))
      contentsElement.append(@createSeparatorElement())

    editable = @simulator.isDocumentEditable()
    contentsElement.append([
      @createActionElement "Duplicate...", true, () => @actions.duplicateDocument()
      @createActionElement "Rename...", editable, () => @actions.renameDocument()
      @createActionElement "Delete...", editable && (@datastore.list().length > 1), () => @actions.deleteDocument()
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
    @simulator.loadedDocumentName

  duplicateDocument: () ->
    loadedDocumentName = @loadedDocumentName()
    newName = @datastore.defaultDuplicateDocumentName(loadedDocumentName)
    newName = window.prompt("New document name", newName)
    if newName != null
      @simulator.saveDocumentAs(newName)
      @simulator.loadDocument(@datastore, newName)

  renameDocument: () ->
    loadedDocumentName = @loadedDocumentName()
    newName = window.prompt("New document name", loadedDocumentName)
    if newName != null
      newName = @datastore.rename(loadedDocumentName, newName)
      @loadDocument(@datastore, newName)

  deleteDocument: () ->
    ok = window.confirm([
      "You are about to delete the document."
      "This cannot be undone."
    ].join(" "))
    if ok
      @datastore.remove(@loadedDocumentName())
      @loadDocument(@datastore, @datastore.list()[0])

  loadDocument: (library, name) ->
    @saveDocumentIfDirty()
    @simulator.loadDocument(library, name)

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

loadVisualizers = (library, map) ->
  library.write(name, code) for name, code of map

module.exports = class Simulator
  constructor: () ->
    @piano = new SimulatorPiano()
    @activeLibrary = new InMemoryVisualizerLibrary()
    @tutorialLibrary = new InMemoryVisualizerLibrary()
    @dataStore = new LocalStorageVisualizerLibrary()
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

    @loadServerVisualizers()
    @loadedDocumentName = @dataStore.loadedDocumentName()
    if @loadedDocumentName == null
      @loadedDocumentName = @activeLibrary.list()[0]
      @loadDocument(@activeLibrary, @loadedDocumentName)
    else
      @loadDocument(@dataStore, @loadedDocumentName)

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
      new TitleBarDropdown($("#title_bar"), this, @actions)

  loadServerVisualizers: () ->
    $.ajax({
      url: "/visualizers",
      async: false
    }).done (content) =>
      loadVisualizers(@activeLibrary, content.activeVisualizers)
      loadVisualizers(@tutorialLibrary, content.tutorialVisualizers)

  loadDocument: (library, name) ->
    console.log("loading #{name} from #{library}")
    content = library.read(name)
    $("#title_label").text(name)
    @editor.setContent(content)
    if library == @dataStore
      @dataStore.setLoadedDocumentName(name)
    @loadedDocumentName = name
    @loadedDocumentLibrary = library
    @setEditorActive(@isDocumentEditable())
    @isDirty = false

  isDocumentEditable: () ->
    @loadedDocumentLibrary == @dataStore

  saveDocument: () ->
    if @loadedDocumentName == null then throw "no document loaded"
    @saveDocumentAs(@loadedDocumentName)

  saveDocumentAs: (name) ->
    console.log("Saving document #{name}")
    @dataStore.write(name, @editor.content())
    console.log("Saved document #{name}")
    @isDirty = false

  saveDocumentIfDirty: () ->
    if @loadedDocumentName != null && @isDirty then @saveDocument()

  runCode: () ->
    # save before running since it's possible the code will crash the app
    @saveDocumentIfDirty()

    code = @editor.content()

    visualizer =
        VisualizerCompiler.instantiate(code, @piano.strip, @piano.pianoKeys)

    @piano.setVisualizer(visualizer)

  setEditorActive: (value) ->
    $("#read_only_message").css("visibility", if value then "hidden" else "visible")
    @editor.setActive(value)

  toggleTitleBar: () ->
