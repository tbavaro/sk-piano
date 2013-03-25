#!/usr/bin/env ruby

require "./PianoDriver"
require "./LightStrips"
require "./TwinkleVisualizer"

class Piano
  def initialize
    @driver = PianoDriver.new
    targetFps = 30
    @targetFrameDuration = 1.0 / targetFps
    @showFpsEveryNFrames = targetFps * 3
    @numPixels = 686
    @prevFrameTime = 0
  end

  def scan
    @driver.sendMessage("SCAN")
    resultCmd, resultBody = @driver.receiveMessageWithTimeout(5)
    abort "result timeout" if resultCmd == nil
    abort "wrong result" if resultCmd != "KEYS"
    resultBody.unpack("c*")
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
    lightStrip = FrameBufferLightStrip.new(@numPixels)
    visualizer = TwinkleVisualizer.new(lightStrip)
    lightStrip.reset
    while true
      # scan to see which keys are pressed
      pressedKeys = scan
      visualizer.setPressedKeys(pressedKeys)

      # send the LED values
      @driver.sendMessage("SHOW", lightStrip.pixels.pack("L*"))
      
      throttle
      
      if (counter % @showFpsEveryNFrames == 0)
        now = Time.now.to_f
        if (lastFpsTime != 0)
          fps = @showFpsEveryNFrames / (now - lastFpsTime)
          puts "fps: #{fps}"
        end
        lastFpsTime = now
      end
      counter += 1
    end
  end
end

piano = Piano.new
piano.loop


