assert = require("assert")
ioctl = require("./ioctl-enhanced")
fs = require("fs")


SPI_DEVICE = "/dev/spidev2.0"


class SPI
  constructor: (@maxSpeedHz) ->
    @fd = fs.openSync(SPI_DEVICE, "w+")

    # make sure we can read the read mode; in the C++ version we
    # assert that it's 0 but it seems to be 1 with the new version
    # of angstrom linux (even in C++) so who knows?
    ioctl.ioctlSync8(@fd, SPI_IOC_RD_MODE)

    bitsPerWord = ioctl.ioctlSync8(@fd, SPI_IOC_RD_BITS_PER_WORD)
    assert.equal(8, bitsPerWord)


    console.log("bitsPerWord: " + bitsPerWord)

  close: () ->
    fs.closeSync(@fd)

module.exports = SPI

