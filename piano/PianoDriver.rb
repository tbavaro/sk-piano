require "pty"

class PianoDriver
  def initialize
    inPipe = "/tmp/piano.#{Process.pid}.in"
    outPipe = "/tmp/piano.#{Process.pid}.out"
    `mkfifo #{inPipe}`
    `mkfifo #{outPipe}`
    @w = open(outPipe, "wb+")
    @r = open(inPipe, "rb+")
    @subprocess = IO.popen(["../driver/driver", outPipe, inPipe], mode = "w")
    begin
      abort "can't connect" if receiveMessageWithTimeout(5) != ["OK", ""]
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

  def peekMessage
    receiveMessageWithTimeout(0)
  end

  def receiveMessageWithTimeout(timeout)
    if select([@r], [], [], timeout) == nil
      nil
    else
      receiveMessage
    end
  end

  def receiveMessage
    length = _readBytes(4).unpack("L")[0]
    _readBytes(length).unpack("A4a*")
  end

  def sendMessage(cmd, body = "")
    @w.write([body.length + 4, cmd, body].pack("LA4a*"))
    @w.flush
  end
end

