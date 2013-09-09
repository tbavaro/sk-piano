Spi = require("spi")

spi = new Spi("/dev/spidev2.0", 4e6)
buf = new Buffer(32)

for i in [0...buf.length] by 1
  buf[i] = i

console.log(buf.length)
spi.send(buf)
