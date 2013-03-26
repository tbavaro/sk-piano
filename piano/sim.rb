#!/usr/bin/env ruby

require "./Colors"
require "./Piano"

require "rubygems"
require "em-websocket"
require "socket"

PORT = 8001

class SimulatorPiano < Piano
  def initialize(ws)
    super()
    @ws = ws
    @mutex = Mutex.new
    @keys = Array.new(88, false)
  end

  def pressedKeys
    @mutex.synchronize do
      result = []
      @keys.each_index { |i| result.push(i) if @keys[i] }
      result
    end
  end

  def onKeyDown(key)
    @mutex.synchronize { @keys[key] = true }
  end

  def onKeyUp(key)
    @mutex.synchronize { @keys[key] = false }
  end

  def setLeds(pixels)
    def serialize(pixel)
      def gammaCorrect(v)
        (127.0 * ((v / 127.0) ** 0.5)).floor
      end

      "'%02x%02x%02x'" % [
        gammaCorrect(Colors.red(pixel)) * 2,
        gammaCorrect(Colors.green(pixel)) * 2,
        gammaCorrect(Colors.blue(pixel)) * 2
      ]
    end

    @ws.send("SHOW:[" + (pixels.map { |pixel| serialize(pixel) }.join(",")) + "]")
  end
end

puts "Listening on port #{PORT}..."

EventMachine::WebSocket.start(:host => "0.0.0.0", :port => PORT) do |ws|
  thread = nil
  piano = nil

  ws.onopen do
    puts "Connected!"
    thread = Thread.new do
      begin
        piano = SimulatorPiano.new(ws)
        piano.loop
      rescue => e
        puts "EXCEPTION:\n#{e}\n#{e.backtrace.join("\n")}"
      end
    end
  end

  ws.onmessage do |msg|
    cmd, body = msg.split(":", 2)
    case cmd
    when "KEY_DOWN"
      piano.onKeyDown(body.to_i)
    when "KEY_UP"
      piano.onKeyUp(body.to_i)
    else
      abort "invalid message: #{msg}"
    end
  end

  ws.onclose do
    puts "Closed"
    Thread.kill(thread)
    thread = nil
    piano = nil
  end
end
