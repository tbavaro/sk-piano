#!/usr/bin/env ruby

require "rubygems"
require "em-websocket"
require "socket"


class FooConnection < EventMachine::Connection
  def initialize(ws)
    @ws = ws
    @remaining = ""
  end
  attr_reader :ws
  def post_init
#    ws.send("started")
  end

  def receive_data data
    while data.index("\n") != nil
      line, data = data.split("\n", 2)
#      puts "#{@remaining}#{line}"
      ws.send("#{@remaining}#{line}")
      @remaining = ""
    end
    @remaining = data
  end

  def unbind
    status = get_status.exitstatus;
    puts "Program terminated: #{status}"
    ws.send("Program terminated: #{status}")
    ws.close_connection()
  end
end
  
puts "Listening!"
EventMachine::WebSocket.start(:host => "0.0.0.0", :port => 8001) do |ws|
  p = nil

  ws.onopen { 
    puts "Connected!"
    p = EventMachine.popen("../beaglebone/piano", FooConnection, ws)
  }

  ws.onmessage do |msg|
    puts "got #{msg}"
    p.send_data "#{msg}\n"
  end

  ws.onclose {
    p.close_connection()
    puts "Closed" 
  }
    
end
 
  
