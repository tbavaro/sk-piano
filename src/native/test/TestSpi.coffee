Spi = require("../build/Release/piano_native").Spi

spi = new Spi("/dev/spidev2.0", 4e6)
buf = new Buffer(32)

for i in [0...buf.length] by 1
  buf[i] = i

spi.send(buf)
