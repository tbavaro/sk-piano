SPI = require("./spi")

spi = new SPI(115200)
console.log("speed: " + spi.maxSpeedHz)

spi.close()
