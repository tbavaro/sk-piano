TBBeagleBone = require("../../piano_node/TBBeagleBone")

NOTE_PINS = [15, 13, 23, 21, 26, 24, 19, 17, 25, 18, 20, 22]
OCTAVE_PINS = [12, 14, 16, 7, 9, 11, 10, 8]

outputPins = (TBBeagleBone.Pins.pin(8, i) for i in OCTAVE_PINS)
inputPins = (TBBeagleBone.Pins.pin(8, i) for i in NOTE_PINS)

pinScanner = new TBBeagleBone.PinScanner(outputPins, inputPins)
start = Date.now()
for i in [0...100] by 1
  result = pinScanner.scan()
#  console.log("result:", result)
end = Date.now()
console.log(end - start)
