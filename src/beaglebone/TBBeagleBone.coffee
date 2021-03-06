fs = require("fs")
PianoNative = require("piano_native")

IS_BEAGLEBONE = (process.arch == "arm")

MMap = PianoNative.MMap

GPIO_BANK_OFFSETS = [
  0x44E07000,
  0x4804C000,
  0x481AC000
]
GPIO_SIZE = 0x1000
GPIO_OE = 0x134
GPIO_DATAIN = 0x138
GPIO_DATAOUT = 0x13C
GPIO_SETDATAOUT = 0x194
GPIO_CLEARDATAOUT = 0x190

getOrOpenDevMem = (() ->
  fd = null
  () -> fd ||= fs.openSync("/dev/mem", "r+")
)()

bclr = (buffer, offset, bit) ->
  value = buffer.readInt32LE(offset)
  value = value & (0xFFFFFFFF - (1 << bit))
  buffer.writeInt32LE(value, offset)

bset = (buffer, offset, bit) ->
  value = buffer.readInt32LE(offset)
  value = value | (1 << bit)
  buffer.writeInt32LE(value, offset)

Gpio =
  Mode:
    INPUT: "INPUT",
    OUTPUT: "OUTPUT"

GPIO_BANK_CACHE = {}
getOrCreateBankMmap = (bankNumber) ->
  GPIO_BANK_CACHE[bankNumber] ||= MMap.map(
      GPIO_SIZE,
      MMap.PROT_READ | MMap.PROT_WRITE,
      MMap.MAP_SHARED,
      getOrOpenDevMem(),
      GPIO_BANK_OFFSETS[bankNumber] || (throw "invalid bank number"))

GPIO_PIN_CACHE = {}
gpioPinCacheKey = (bankNumber, pinNumber) ->
  [bankNumber, pinNumber].join(":")

class GpioPin
  @getOrCreate: (bankNumber, pinNumber) ->
    key = gpioPinCacheKey(bankNumber, pinNumber)
    GPIO_PIN_CACHE[key] ||= new GpioPin(bankNumber, pinNumber)

  constructor: (bankNumber, pinNumber) ->
    if gpioPinCacheKey(bankNumber, pinNumber) in GPIO_PIN_CACHE
      throw "already made this pin; use GpioPin.getOrCreate!"

    @pinNumber = pinNumber
    @_mmap = getOrCreateBankMmap(bankNumber)
    @_pinMask = (1 << pinNumber)

  setMode: (mode) ->
    if mode == Gpio.Mode.INPUT
      bset(@_mmap, GPIO_OE, @pinNumber)
    else
      bclr(@_mmap, GPIO_OE, @pinNumber)
    return

  write: (value) ->
    offset = (if value then GPIO_SETDATAOUT else GPIO_CLEARDATAOUT)
    @_mmap.writeInt32LE(@_pinMask, offset)
    return

  read: () ->
    !!(@_mmap.readInt32LE(GPIO_DATAIN) & @_pinMask)

# to run _most_ of the beaglebone env on mac
class FakeGpioPin
  constructor: (id) ->
    @id = id

  setMode: (mode) ->
    #console.log("Setting pin #{@id} to mode #{mode}")
    return

  write: (value) ->
    #valueStr = (if value then "ON" else "OFF")
    #console.log("Writing pin #{@id} -> #{valueStr}")
    return

  read: () ->
    #console.log("Reading pin #{@id}")
    false
    
HEADER_8_PIN_IDS = [
  null,

  # 1:
  null, null,   38,   39,   34,

  # 6:
  null,   66,   67,   69,   68,

  # 11:
    45,   44,   23,   26,   47,

  # 16:
    46,   27,   65,   22, null,

  # 21:
  null, null, null, null, null,

  # 26:
  null,   86,   88,   87,   89,

  # 31:
  null, null, null, null, null,

  # 36:
  null, null, null,   76,   77,

  # 41:
    74,   75,   72,   73,   70,

  # 46:
    71
]

if IS_BEAGLEBONE
  getOrCreatePinByHeaderAndPinNumber = (headerNumber, pinNumber) ->
    id = (if headerNumber != 8 then null else HEADER_8_PIN_IDS[pinNumber])
    throw ("invalid header/pin: " + headerNumber + "/" + pinNumber) if !id
    bankNumber = Math.floor(id / 32)
    bankPinNumber = id % 32
    GpioPin.getOrCreate(bankNumber, bankPinNumber)
else
  getOrCreatePinByHeaderAndPinNumber = (headerNumber, pinNumber) ->
    new FakeGpioPin("#{headerNumber}:#{pinNumber}")

module.exports =
  Spi: PianoNative.Spi
  Pin: getOrCreatePinByHeaderAndPinNumber

module.exports.Pin.INPUT = Gpio.Mode.INPUT
module.exports.Pin.OUTPUT = Gpio.Mode.OUTPUT
