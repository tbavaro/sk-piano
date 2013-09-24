IS_DVORAK = true

NUM_WHITE_KEYS = 52
BLACK_KEY_INDEXES = [ 0, 2, 3, 5, 6 ]

WHITE_NOTE_OFFSETS = (() ->
  results = []
  j = 0
  for i in [0...12] by 1
    results.push(j++)
    j++ if i in BLACK_KEY_INDEXES
  results
)()

noteFromKeyCode = (() ->
  LOWEST_MAPPED_KEY = 39 # C3
  INVERSE_MAPPING_DVORAK = [186,79,81,69,74,75,73,88,68,66,72,77,87,78,86,83,90,222,50,188,51,190,52,80,89,54,70,55,71,67,57,82,48,76,219,191,187]
  INVERSE_MAPPING_QWERTY = [90,83,88,68,67,86,71,66,72,78,74,77,188,76,190,186,191,81,50,50,87,51,69,52,82,84,54,89,55,85,73,57,79,48,80,189,219,221]
  inverseMapping = (if IS_DVORAK then INVERSE_MAPPING_DVORAK else INVERSE_MAPPING_QWERTY)
  keyMapping = new Array(256)
  keyMapping[i] = null for i in [0...keyMapping.length] by 1
  for i in [0...inverseMapping.length] by 1
    keyMapping[inverseMapping[i]] = i + LOWEST_MAPPED_KEY
  keyMapping[13] = 0

  (keyCode) ->
    if keyCode >= 0 && keyCode <= keyMapping.length
      keyMapping[keyCode]
    else
      null
)()

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
        $v(e.target).focus()
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
      note = (octave * 12 + WHITE_NOTE_OFFSETS[index])

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
        note = (octave * 12 + WHITE_NOTE_OFFSETS[index] + 1)

        element = document.createElement("div")
        element.className = "piano-key black"
        element.style.width = "#{black_key_width_pct}%"
        element.style.left = "calc(#{(i + 1) * key_width_pct - (black_key_width_pct / 2)}% + 0.5px)"

        addNoteElement(note, element)

    $(window).keydown (e) =>
      if e.target.nodeName != "TEXTAREA"
        note = noteFromKeyCode(e.keyCode)
        @onNoteDown(note) if note != null

    $(window).keyup (e) =>
      if e.target.nodeName != "TEXTAREA"
        note = noteFromKeyCode(e.keyCode)
        @onNoteUp(note) if note != null

  onNoteDown: (note) ->
    $(@noteElements[note]).addClass("down")

  onNoteUp: (note) ->
    $(@noteElements[note]).removeClass("down")

module.exports = PianoKeyboard
