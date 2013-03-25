#!/usr/bin/env ruby

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
end

require "pty"

class RealPiano < Piano
  def initialize
    super
    inPipe = "/tmp/piano.#{Process.pid}.in"
    outPipe = "/tmp/piano.#{Process.pid}.out"
    `mkfifo #{inPipe}`
    `mkfifo #{outPipe}`
    @w = open(outPipe, "wb+")
    @r = open(inPipe, "rb+")
    @subprocess = IO.popen(["../driver/driver", outPipe, inPipe], mode = "w")
    begin
      abort "can't connect" if _receiveMessageWithTimeout(5) != ["OK", ""]
    ensure
      File.delete inPipe
      File.delete outPipe
    end
  end

  def _readBytes(n)
    bytes = @r.read(n)
    abort "EOF" if bytes.length != n
    bytes
  end

  def _peekMessage
    receiveMessageWithTimeout(0)
  end

  def _receiveMessageWithTimeout(timeout)
    if select([@r], [], [], timeout) == nil
      nil
    else
      _receiveMessage
    end
  end

  def _receiveMessage
    length = _readBytes(4).unpack("L")[0]
    _readBytes(length).unpack("A4a*")
  end

  def _sendMessage(cmd, body = "")
    @w.write([body.length + 4, cmd, body].pack("LA4a*"))
    @w.flush
  end
  
  def pressedKeys
    _sendMessage("SCAN")
    resultCmd, resultBody = _receiveMessageWithTimeout(5)
    abort "result timeout" if resultCmd == nil
    abort "wrong result" if resultCmd != "KEYS"
    resultBody.unpack("c*")
  end

  def setLeds(pixels)
    # send the LED values.  These will be in the same order we should
    # send them to the LED strip, except we pack them as 4 byte 
    # network-order values but the LED strip just wants 3 bytes per pixel
    # so we'll need to strip out the leading 0 byte for each pixel
    _sendMessage("SHOW", pixels.pack("N*"))
  end
end

piano = RealPiano.new
counter = 0
lastFpsTime = 0
showFpsEveryNFrames = TARGET_FPS * 3
lightStrip = FrameBufferLightStrip.new(NUM_PIXELS)
visualizer = TwinkleVisualizer.new(lightStrip)
lightStrip.reset
while true
  # scan to see which keys are pressed
  visualizer.setPressedKeys(piano.pressedKeys)

  # set the LEDs
  piano.setLeds(lightStrip.pixels)

  # sleep to keep the FPS consistent
  piano.throttle
  
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

