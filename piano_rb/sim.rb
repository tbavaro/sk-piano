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
    @onscreen_color_hex = (0..127).map do |v|
      # do some gamma correction since the LEDs are anything but linear in
      # intensity
      "%02x" % ((255.0 * ((v / 127.0) ** 0.4)).floor)
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
      "'%s%s%s'" % [
        @onscreen_color_hex[Colors.red(pixel) * 127],
        @onscreen_color_hex[Colors.green(pixel) * 127],
        @onscreen_color_hex[Colors.blue(pixel) * 127]
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
