Piano = require("Piano")
MasterVisualizer = require("./MasterVisualizer")
PhysicalLightStrip = require("./PhysicalLightStrip")
PhysicalPianoKeys = require("./PhysicalPianoKeys")
TBBeagleBone = require("./TBBeagleBone")

Sleep = require("sleep")

SPI_DEVICE = "/dev/spidev2.0"
SPI_FREQUENCY_HZ = 4e6
NUM_FRAMES_TO_RUN_SYNC = Piano.TARGET_FPS * 5

class RealPiano extends Piano
  constructor: () ->
    spi = new TBBeagleBone.Spi(SPI_DEVICE, SPI_FREQUENCY_HZ)
    strip = new PhysicalLightStrip(spi, Piano.NUM_PIXELS)
    pianoKeys = new PhysicalPianoKeys()
    visualizer = new MasterVisualizer(strip, pianoKeys)
    super(strip, pianoKeys, visualizer)

  # on the actual device, setTimeout is very imprecise; do a bunch of frames
  # synchronously and periodically reschedule with setTimeout so we're not
  # actually blocking forever
  run: () ->
    # run N-1 frames synchronously
    for _ in [0...(NUM_FRAMES_TO_RUN_SYNC - 1)] by 1
      delay = @runLoopOnce()
      if delay > 0
        Sleep.usleep(delay * 1000)

    # run 1 more frame but delay via setTimeout so we give node a chance to
    # do any extra tasks it needs to do
    delay = @runLoopOnce()
    setTimeout(@run.bind(this), delay);

    return

(new RealPiano).run()
