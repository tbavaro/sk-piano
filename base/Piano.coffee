Colors = require("Colors")
MasterVisualizer = require("MasterVisualizer")

class Piano
  @TARGET_FPS: 30
  @NUM_PIXELS: 686

  constructor: (strip, pianoKeys) ->
    @strip = strip
    @pianoKeys = pianoKeys
    @visualizer = new MasterVisualizer(strip, pianoKeys)
    @targetFrameDuration = 1000.0 / Piano.TARGET_FPS
    @prevFrameTime = Date.now()
    @nextFrameTime = @prevFrameTime
    @logEveryNFrames = Piano.TARGET_FPS * 3
    @resetLogState()

  runLoopOnce: () ->
    startTime = Date.now()
    secondsSinceLastFrame = (startTime - @prevFrameTime) / 1000.0
    @prevFrameTime = startTime

    @pianoKeys.scan();
    @visualizer.render(secondsSinceLastFrame)
    @strip.display()

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

  run: () ->
    delay = 0
    while delay == 0
      delay = @runLoopOnce()
    setTimeout(@run.bind(this), delay)
    return

  resetLogState: (opt_now) ->
    now = opt_now || Date.now()
    @framesUntilLog = @logEveryNFrames
    @logWindowStartTime = now
    @logWindowRenderingTime = 0
    return

  logStats: (opt_now) ->
    now = opt_now || Date.now()
    logWindowDurationSeconds = (now - @logWindowStartTime) / 1000.0
    fps = @logEveryNFrames / logWindowDurationSeconds
    averageRenderTime = (1.0 * @logWindowRenderingTime / @logEveryNFrames)
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

module.exports = Piano
