NUM_WHITE_KEYS = 52
BLACK_KEY_INDEXES = [ 0, 2, 3, 5, 6 ]

white_note_offsets = []
j = 0
for i in [0...12] by 1
  white_note_offsets.push(j++)
  j++ if i in BLACK_KEY_INDEXES

class PianoKeyboard
  constructor: (viewportDomElement) ->
    key_width_pct = 100 / NUM_WHITE_KEYS
    black_key_width_pct = key_width_pct * 0.7

    @noteElements = new Array(88)

    mouseDown = false
    $(window).mouseup () => mouseDown = false

    addNoteElement = (note, element) =>
      viewportDomElement.appendChild(element)
      @noteElements[note] = element

      $(element).mousedown (e) =>
        mouseDown = true
        @onNoteDown(note)
        e.preventDefault()
      $(element).mouseup (e) =>
        @onNoteUp(note) if mouseDown
        mouseDown = false
      $(element).mouseenter () =>
        @onNoteDown(note) if mouseDown
      $(element).mouseleave () =>
        @onNoteUp(note) if mouseDown

    # white keys
    for i in [0...NUM_WHITE_KEYS] by 1
      octave = Math.floor(i / 7)
      index = i % 7
      note = (octave * 12 + white_note_offsets[index])

      element = document.createElement("div")
      element.className = "piano-key white"
      element.style.width = "#{key_width_pct}%"
      element.style.left = "calc(#{i * key_width_pct}% + 0.5px)"

      addNoteElement(note, element)

    # black keys
    for i in [0...(NUM_WHITE_KEYS - 1)] by 1
      index = i % 7
      if (index in BLACK_KEY_INDEXES)
        octave = Math.floor(i / 7)
        note = (octave * 12 + white_note_offsets[index] + 1)

        element = document.createElement("div")
        element.className = "piano-key black"
        element.style.width = "#{black_key_width_pct}%"
        element.style.left = "calc(#{(i + 1) * key_width_pct - (black_key_width_pct / 2)}% + 0.5px)"

        addNoteElement(note, element)

    console.log(@noteElements)

  onNoteDown: (note) ->
    $(@noteElements[note]).addClass("down")

  onNoteUp: (note) ->
    $(@noteElements[note]).removeClass("down")

module.exports = PianoKeyboard
