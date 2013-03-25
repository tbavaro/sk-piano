#!/usr/bin/env ruby

require "./PianoDriver"

class Piano
  def initialize
    @driver = PianoDriver.new
    targetFps = 30
    @targetFrameDuration = 1.0 / targetFps
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
    count = 0
    while true
      scan
      puts "frame! #{count}"
      count += 1
      throttle
#      msg = @driver.peekMessage 
#      if msg != nil
#        puts ">#{msg}"
#      else
#        @driver.sendMessage("ECHO", "count#{count}")
#        count += 1
#      end
    end
  end
end

piano = Piano.new
piano.loop


