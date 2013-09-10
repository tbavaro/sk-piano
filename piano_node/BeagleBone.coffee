fs = require("fs")
assert = require("assert")

BYTE_ZERO = "0".charCodeAt(0)
BYTE_ONE = "1".charCodeAt(0)

byteBuffer = new Buffer(1)

openAndReadOneByteSync = (filename) ->
  fd = fs.openSync(filename, "r")
  fs.readSync(fd, byteBuffer, 0, 1, null)
  fs.closeSync(fd)
  byteBuffer.readUInt8(0)

writeOneByteSync = (fd, value) ->
  byteBuffer.writeUInt8(value, 0)
  fs.writeSync(fd, byteBuffer, 0, 1, null)
  return

class Pin
  @INPUT:
    direction: "in"
    mode: 27

  @OUTPUT:
    direction: "out"
    mode: 7

  constructor: (name, number) ->
    @name = name
    @number = number
    @_gpioPath = "/sys/class/gpio/gpio" + number
    @_valueFileName = @_gpioPath + "/value"
    @_valueFile = null

  close: () ->
    if @_valueFile != null
      fs.closeSync(@_valueFile)
      @_valueFile = null
    return

  setMode: (mode) ->
    # close the value file if it's open
    @close()

    # export the pin if necessary
    if !fs.existsSync(@_gpioPath)
      fs.writeFileSync("/sys/class/gpio/export", @number.toString())
      assert(fs.existsSync(@_gpioPath))

    # set the direction and mode
    fs.writeFileSync(@_gpioPath + "/direction", mode.direction)
    #fs.writeFileSync("/sys/kernel/debug/omap_mux/" + @name, mode.mode.toString())
    return

  read: () ->
    openAndReadOneByteSync(@_valueFileName) == BYTE_ONE

  write: (value) ->
    # open the file if it's not already open
    fd = @_valueFile
    if fd == null
      @_valueFile = fd = fs.openSync(@_valueFileName, "w")

    writeOneByteSync(fd, if value then BYTE_ONE else BYTE_ZERO)

    # NB this doesn't seem to work...
    #fs.fsyncSync(fd)

    # ...so I guess just close the file to force the flush?
    #@close()

    return

Pins =
  _headers:
    8: [
      null, # there's never a pin 0

      # 1:
      null,
      null,
      new Pin("gpmc_ad6", 38),
      new Pin("gpmc_ad7", 39),
      new Pin("gpmc_ad2", 34),
      
      # 6:
      null, #  new Pin("gpmc_ad3", 35), # something's wrong here, maybe reserved?
      new Pin("gpmc_advn_ale", 66),
      new Pin("gpmc_oen_ren", 67),
      new Pin("gpmc_ben0_cle", 69),
      new Pin("gpmc_wen", 68),
      
      # 11:
      new Pin("gpmc_ad13", 45),
      new Pin("gpmc_ad12", 44),
      new Pin("gpmc_ad9", 23),
      new Pin("gpmc_ad10", 26),
      new Pin("gpmc_ad15", 47),
      
      # 16:
      new Pin("gpmc_ad14", 46),
      new Pin("gpmc_ad11", 27),
      new Pin("gpmc_clk", 65),
      new Pin("gpmc_ad8", 22),
      new Pin("gpmc_csn2", 63),
      
      # 21:
      new Pin("gpmc_csn1", 62),
      new Pin("gpmc_ad5", 37),
      new Pin("gpmc_ad4", 36),
      new Pin("gpmc_ad1", 33),
      new Pin("gpmc_ad0", 32),
      
      # 26:
      new Pin("gpmc_csn0", 61)
    ]
  
  pin: (headerNumber, pinNumber) ->
    header = Pins._headers[headerNumber]
    if !header
      throw ("unsupported header: " + headerNumber)

    pin =
      if pinNumber < 1 || pinNumber >= header.length
        null
      else header[pinNumber]
    if !pin
      throw ("unsupported header/pin: " + headerNumber + "/" + pinNumber)

    pin

exports.Pin = Pin
exports.Pins = Pins
exports.Spi = require("../node-spi/build/Release/spi")
