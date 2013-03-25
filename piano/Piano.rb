require "./LightStrips"
require "./TwinkleVisualizer"

TARGET_FPS = 30
NUM_PIXELS = 686

class Piano
  def initialize
    @targetFrameDuration = 1.0 / TARGET_FPS
    @prevFrameTime = 0
  end

  def pressedKeys
    throw "abstract"
  end

  def setLeds(pixels)
    throw "abstract"
  end

  def throttle
    now = Time.now.to_f
    diff = (@prevFrameTime + @targetFrameDuration) - now
    sleep diff if diff > 0
    @prevFrameTime = Time.now.to_f
  end

  def loop
    counter = 0
    lastFpsTime = 0
    showFpsEveryNFrames = TARGET_FPS * 3
    lightStrip = FrameBufferLightStrip.new(NUM_PIXELS)
    visualizer = TwinkleVisualizer.new(lightStrip)
    lightStrip.reset
    while true
      # scan to see which keys are pressed
      visualizer.setPressedKeys(pressedKeys)

      # set the LEDs
      setLeds(lightStrip.pixels)

      # sleep to keep the FPS consistent
      throttle
      
      if (counter % showFpsEveryNFrames == 0)
        now = Time.now.to_f
        if (lastFpsTime != 0)
          fps = showFpsEveryNFrames / (now - lastFpsTime)
          puts "fps: #{fps}"
        end
        lastFpsTime = now
      end
      counter += 1
    end
  end
end

