fs = require("fs")
PianoNative = require("../node-tbbeaglebone/build/Release/piano_native")

Mmap =
  map: TBBeagleBoneNative.map
  PROT_READ: TBBeagleBoneNative.PROT_READ
  PROT_WRITE: TBBeagleBoneNative.PROT_WRITE
  PROT_EXEC: TBBeagleBoneNative.PROT_EXEC
  PROT_NONE: TBBeagleBoneNative.PROT_NONE
  MAP_SHARED: TBBeagleBoneNative.MAP_SHARED
  MAP_PRIVATE: TBBeagleBoneNative.MAP_PRIVATE
  PAGESIZE: TBBeagleBoneNative.PAGESIZE

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
    INPUT: 0,
    OUTPUT: 1

GPIO_BANK_CACHE = {}
getOrCreateBankMmap = (bankNumber) ->
  GPIO_BANK_CACHE[bankNumber] ||= Mmap.map(
      GPIO_SIZE,
      Mmap.PROT_READ | Mmap.PROT_WRITE,
      Mmap.MAP_SHARED,
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

module.exports =
  Spi: TBBeagleBoneNative.Spi
  Pin: GpioPin.getOrCreate

module.exports.Pin.INPUT = Gpio.Mode.INPUT
module.exports.Pin.OUTPUT = Gpio.Mode.OUTPUT
