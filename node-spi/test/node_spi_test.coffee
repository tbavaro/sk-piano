Spi = require("spi")

spi = new Spi("/dev/spidev2.0", 4e6)
buf = new Buffer(32)
spi.send(buf)

