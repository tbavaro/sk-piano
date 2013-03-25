class Piano
  def pressedKeys
    throw "abstract"
  end

  def setLeds(pixels)
    throw "abstract"
  end
end

require "pty"

class RealPiano < Piano
  def initialize
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
