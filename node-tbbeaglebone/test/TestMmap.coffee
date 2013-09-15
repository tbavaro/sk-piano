fs = require("fs")
sleep = require("sleep")
TBBeagleBone = require("../build/Release/tbbeaglebone")

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

fd = fs.openSync("/dev/mem", "r")
m = TBBeagleBone.map(
    GPIO_SIZE,
    TBBeagleBone.PROT_READ | TBBeagleBone.PROT_WRITE,
    TBBeagleBone.MAP_SHARED,
    fd,
    GPIO_BANK_OFFSETS[1])

bclr = (buffer, offset, bit) ->
  value = buffer.readInt32BE(offset)
  value = value & (0xFFFFFFFF - (1 << bit))
  buffer.writeInt32BE(value, offset)

bset = (buffer, offset, bit) ->
  value = buffer.readInt32BE(offset)
  value = value | (1 << bit)
  console.log(value)
  buffer.writeInt32BE(value, offset)

bitsToString = (val) ->
  ((if val & 1 << i then 1 else 0) for i in [31..0] by -1).join("")

class Pin
  constructor: (pinNumber) ->
    @pinNumber = pinNumber
    @pinMask = (1 << pinNumber)

  setMode: (output) ->
    if output
      bclr(m, GPIO_OE, @pinNumber)
    else
      bset(m, GPIO_OE, @pinNumber)
    console.log("oe: " + bitsToString(m.readInt32BE(GPIO_OE)))
    return

  write: (value) ->
    offset = (if value then GPIO_SETDATAOUT else GPIO_CLEARDATAOUT)
    m.writeInt32BE(@pinMask, offset)
    console.log("bits: " + bitsToString(m.readInt32BE(GPIO_DATAOUT)))
    return

  read: () ->
    !!(m.readInt32BE(GPIO_DATAIN) & @pinMask)

pins = (new Pin(i) for i in [21..24])
pin.setMode(true) for pin in pins

while true
  pin.write(true) for pin in pins
  sleep.sleep(1.0)
  pin.write(false) for pin in pins
  sleep.sleep(1.0)
