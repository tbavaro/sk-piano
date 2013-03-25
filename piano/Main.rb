#!/usr/bin/env ruby

require "./Piano"
require "./LightStrips"
require "./TwinkleVisualizer"

class PianoApp
  def initialize(piano)
    @piano = piano
    targetFps = 30
    @targetFrameDuration = 1.0 / targetFps
    @showFpsEveryNFrames = targetFps * 3
    @numPixels = 686
    @prevFrameTime = 0
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
      visualizer.setPressedKeys(@piano.pressedKeys)

      # set the LEDs
      @piano.setLeds(lightStrip.pixels)

      # sleep to keep the FPS consistent
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

PianoApp.new(RealPiano.new).loop


