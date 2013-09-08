SPI = require("./build/Release/spi");

spi = new SPI.spi("/dev/spidev2.0", 4e6);

buf = new Buffer(32);
spi.send(buf);

