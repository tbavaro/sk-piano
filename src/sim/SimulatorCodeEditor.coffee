class SimulatorCodeEditor
  constructor: (textArea) ->
    @codeMirror = CodeMirror.fromTextArea(textArea, {
      mode: "coffeescript"
      tabSize: 2
      autofocus: true
      indentWithTabs: true
      highlightSelectionMatches: {}
      #        showToken: true
    })

module.exports = SimulatorCodeEditor
