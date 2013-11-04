PianoKeys = require("lib/PianoKeys")
TBBeagleBone = require("./TBBeagleBone")
assert = require("assert")
Sleep = require("sleep")

PIN_HEADER = 8
NOTE_PINS = [15, 13, 23, 21, 26, 24, 19, 17, 25, 18, 20, 22]
OCTAVE_PINS = [12, 14, 16, 7, 9, 11, 10, 8]

# We didn't take a lot of care to get the note wiring in the exact same order
# for every octave, so we remap them here.
#
# key_mappings[i] = note where:
#   i = octavePinIndex * 12 + notePinIndex ("unmapped key")
#   note = note value from 0 to (Piano.NUM_KEYS - 1)
KEY_MAPPINGS = [
   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,
  13,  12,  16,  14,  23,  22,  18,  15,  17,  19,  21,  20,
  25,  24,  34,  35,  30,  28,  31,  32,  33,  26,  27,  29,
  44,  47,  41,  45,  37,  38,  39,  40,  36,  43,  46,  42,
  59,  58,  50,  56,  53,  48,  52,  57,  51,  55,  49,  54,
  71,  70,  63,  60,  66,  65,  64,  61,  62,  67,  68,  69,
  82,  83,  73,  74,  78,  81,  75,  76,  72,  80,  79,  77,
  -1,  -1,  -1,  -1,  85,  86,  -1,  -1,  -1,  -1,  84,  87
]

class PhysicalPianoKeys extends PianoKeys
  constructor: () ->
    super
    @_notePins = (TBBeagleBone.Pin(PIN_HEADER, n) for n in NOTE_PINS)
    @_octavePins = (TBBeagleBone.Pin(PIN_HEADER, n) for n in OCTAVE_PINS)

    # set up note pins for INPUT pins and octave pins for OTPUT
    pin.setMode(TBBeagleBone.Pin.INPUT) for pin in @_notePins
    pin.setMode(TBBeagleBone.Pin.OUTPUT) for pin in @_octavePins

    # turn off all of the octave pins
    pin.write(false) for pin in @_octavePins

  scan: () ->
    pressedKeys = []

    unmappedKey = 0
    for octavePin in @_octavePins
      # turn on the current octave and give it 1 ms to take effect
      octavePin.write(true)
      Sleep.usleep(100)

      for notePin in @_notePins
        if notePin.read()
          pressedKeys.push(KEY_MAPPINGS[unmappedKey])
        ++unmappedKey

      # turn off this octave
      octavePin.write(false)

    @setPressedKeys(pressedKeys)
    return

  monitor: () ->
    @scan()
    console.log("scan")
    if @pressedSinceLastFrame.length > 0
      console.log("PRESSED: " + @pressedSinceLastFrame)
    if @releasedSinceLastFrame.length > 0
      console.log("RELEASED: " + @releasedSinceLastFrame)
    setTimeout (() => @monitor()), 10
    return

module.exports = PhysicalPianoKeys
