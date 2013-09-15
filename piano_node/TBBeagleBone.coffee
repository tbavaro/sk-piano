TBBeagleBone = require("../node-tbbeaglebone/build/Release/tbbeaglebone")

TBBeagleBone.Pin.Modes =
  INPUT: 0
  OUTPUT: 1
  
CACHED_HEADERS = {}

TBBeagleBone.Pins =
  pin: (headerNumber, pinNumber) ->
    cachedPins = CACHED_HEADERS[headerNumber]
    if !cachedPins
      cachedPins = CACHED_HEADERS[headerNumber] = {}

    pin = cachedPins[pinNumber]
    if !pin
      pin = cachedPins[pinNumber] =
          new TBBeagleBone.Pin(headerNumber, pinNumber)

    pin

module.exports = TBBeagleBone
