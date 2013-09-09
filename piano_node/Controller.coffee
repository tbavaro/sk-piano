Colors = require("./Colors")
FrameBufferLightStrip = require("./FrameBufferLightStrip")
MasterVisualizer = require("./MasterVisualizer")
PianoKeys = require("./PianoKeys")
Spi = require("../node-spi/build/Release/spi")

SPI_DEVICE = "/dev/spidev2.0"
SPI_FREQUENCY_HZ = 4e6
TARGET_FPS = 30
LOG_EVERY_N_FRAMES = TARGET_FPS * 3
NUM_PIXELS = 686
IS_SIMULATOR = (process.arch != "arm")

# TODO separate this out to RealController and SimulatorController

class Controller
  constructor: (strip, pianoKeys, visualizer) ->
    @strip = strip
    @pianoKeys = pianoKeys
    @visualizer = visualizer
    @spi = new Spi(SPI_DEVICE, SPI_FREQUENCY_HZ)
    @targetFrameDuration = 1000.0 / TARGET_FPS
    @prevFrameTime = Date.now()
    @nextFrameTime = @prevFrameTime
    @resetLogState()

  runLoopOnce: () ->
    startTime = Date.now()
    secondsSinceLastFrame = (startTime - @prevFrameTime) / 1000.0
    @prevFrameTime = startTime

    # TODO update piano keys

    @visualizer.render(secondsSinceLastFrame)

    @sendBuffer()

    endTime = Date.now()

    @logWindowRenderingTime += (endTime - startTime)
    @framesUntilLog -= 1
    if @framesUntilLog == 0
      @logStats(endTime)
      @resetLogState(endTime)

    @nextFrameTime =
      Math.max(endTime, @nextFrameTime + @targetFrameDuration)

    # return the delay before we should render the next frame
    (@nextFrameTime - endTime)

  # blocks forever but is considerably faster on the actual device
  runLoopSync: () ->
    Sleep = require("sleep")
    while true
      delay = @runLoopOnce()
      if delay > 0
        Sleep.usleep(delay * 1000)
    return

  runLoopAsync: () ->
    delay = 0
    while delay == 0
      delay = @runLoopOnce()
    setTimeout(@runLoopAsync.bind(this), delay)
    return

  sendBuffer: () ->
    @spi.send(@strip.buffer())
    return

  resetLogState: (opt_now) ->
    now = opt_now || Date.now()
    @framesUntilLog = LOG_EVERY_N_FRAMES
    @logWindowStartTime = now
    @logWindowRenderingTime = 0
    return

  logStats: (opt_now) ->
    now = opt_now || Date.now()
    logWindowDurationSeconds = (now - @logWindowStartTime) / 1000.0
    fps = LOG_EVERY_N_FRAMES / logWindowDurationSeconds
    averageRenderTime = (1.0 * @logWindowRenderingTime / LOG_EVERY_N_FRAMES)
    idlePercent = (1.0 - averageRenderTime / @targetFrameDuration) * 100
    console.log([
      fps.toFixed(2),
      "fps; average",
      averageRenderTime.toFixed(2),
      "ms per render;",
      idlePercent.toFixed(2) + "%"
      "idle"
    ].join(" "))
    return

strip = new FrameBufferLightStrip(NUM_PIXELS)
pianoKeys = new PianoKeys()
visualizer = new MasterVisualizer(strip, pianoKeys)
ctrl = new Controller(strip, pianoKeys, visualizer)

if IS_SIMULATOR
  ctrl.runLoopAsync()
else
  ctrl.runLoopSync()
