#!/usr/bin/env ruby

require "./Piano"

class RealPiano < Piano
  def initialize
    super
    in_pipe = "/tmp/piano.#{Process.pid}.in"
    out_pipe = "/tmp/piano.#{Process.pid}.out"
    `mkfifo #{in_pipe}`
    `mkfifo #{out_pipe}`
    @w = open(out_pipe, "wb+")
    @r = open(in_pipe, "rb+")
    @subprocess = IO.popen(["../driver/driver", out_pipe, in_pipe], mode = "w")
    begin
      abort "can't connect" if _receive_message_with_timeout(5) != ["OK", ""]
    ensure
      File.delete in_pipe
      File.delete out_pipe
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

  def _receive_message_with_timeout(timeout)
    if select([@r], [], [], timeout) == nil
      nil
    else
      _receive_message
    end
  end

  def _receive_message
    length = _readBytes(4).unpack("L")[0]
    _readBytes(length).unpack("A4a*")
  end

  def _send_message(cmd, body = "")
    @w.write([body.length + 4, cmd, body].pack("LA4a*"))
    @w.flush
  end
  
  def pressed_keys
    _send_message("SCAN")
    resultCmd, resultBody = _receive_message_with_timeout(5)
    abort "result timeout" if resultCmd == nil
    abort "wrong result" if resultCmd != "KEYS"
    resultBody.unpack("c*")
  end

  def set_leds(pixels)
    # send the LED values.  These will be in the same order we should
    # send them to the LED strip, except we pack them as 4 byte 
    # network-order values but the LED strip just wants 3 bytes per pixel
    # so we'll need to strip out the leading 0 byte for each pixel
    _send_message("SHOW", pixels.pack("N*"))
  end
end

RealPiano.new.loop

