class SimulatorCodeEditor
  constructor: (textArea) ->
    @codeMirror = CodeMirror.fromTextArea(textArea, {
      autofocus: true

      mode: "coffeescript"

      tabSize: 2
      indentWithTabs: false

      highlightSelectionMatches: {}

      # keys
      extraKeys:
        "Tab": "indentMore"
        "Shift-Tab": "indentLess"
    })

    @ignoreChanges = false

  content: () -> @codeMirror.getValue()

  setContent: (content) ->
    @ignoreChanges = true
    @codeMirror.setValue(content)
    @codeMirror.clearHistory()
    @ignoreChanges = false

  setActive: (value) ->
    @codeMirror.setOption("readOnly", if value then false else "nocursor")

  addOnChangeHandler: (handler) ->
    @codeMirror.on "change", () =>
      if !(@ignoreChanges) then handler.apply(null, arguments)

module.exports = SimulatorCodeEditor
