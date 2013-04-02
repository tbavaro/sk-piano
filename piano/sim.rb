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
    @gamma_correction = (0..127).map do |v|
      (127.0 * ((v / 127.0) ** 0.5)).floor
    end
  end

  def pressed_keys
    @mutex.synchronize do
      result = []
      @keys.each_index { |i| result.push(i) if @keys[i] }
      result
    end
  end

  def on_key_down(key)
    @mutex.synchronize { @keys[key] = true }
  end

  def on_key_up(key)
    @mutex.synchronize { @keys[key] = false }
  end

  def set_leds(pixels)
    def serialize(pixel)
      def gamma_correct(v)
        @gamma_correction[v.floor]
      end

      "'%02x%02x%02x'" % [
        gamma_correct(Colors.red(pixel)) * 2,
        gamma_correct(Colors.green(pixel)) * 2,
        gamma_correct(Colors.blue(pixel)) * 2
      ]
    end

    msg = "SHOW:[" + (pixels.map { |pixel| serialize(pixel) }.join(",")) + "]"

    @ws.send(msg)
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
      piano.on_key_down(body.to_i)
    when "KEY_UP"
      piano.on_key_up(body.to_i)
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
