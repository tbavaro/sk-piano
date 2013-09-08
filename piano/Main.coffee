#WebSocketServer = require("ws").Server
#server = new WebSocketServer { port: 8001 }
#server.on("connection", (ws) ->
#  console.log "Connected!"
#
#  ws.on("message", (message) ->
#    console.log("received: %s", message))
#  ws.send("hello"))

Piano = require("./Piano")
Socket = require("net").Socket
BufferPack = require("bufferpack")

class RealPiano extends Piano
  constructor: (socket_file) ->
    super

    @socket = new Socket

    @socket.on("error", (error) ->
      if error.code == "ECONNREFUSED"
        console.log("Unable to connect.", error)
      else
        console.log("Unknown error.", error))

    @socket.on("connect", () ->
      console.log("Connected!"))

    @received_messages = []
    @socket.on("data", (buf) ->
      console.log("received data", buf))

    @socket.connect(socket_file)

  _receiveMessage: () ->
    message = @received_messages.pop()
    if message == undefined then [null, null] else message

  _sendMessage: (cmd, body) ->
    body = body || new Buffer([])

    success = @socket.write(BufferPack.pack("L4s", [body.length + 4, cmd]))
    console.log("success", success);
    @socket.write(body)

  pressedKeys: () ->
    this._sendMessage("SCAN")
    []
#    [result_cmd, result_body] = this._receiveMessage(5)
 #   console.log(result_cmd)
  #  if result_cmd == null then throw "result timeout"
   # if result_cmd != "KEYS" then throw "wrong result"

  setLeds: (pixels) ->
    # send the LED values.  These will be in the same order we should
    # send them to the LED strip, except we pack them as 4 byte
    # network-order values but the LED strip just wants 3 bytes per pixel
    # so we'll need to strip out the leading 0 byte for each pixel
    this._sendMessage("SHOW", BufferPack.pack(pixels.length + "L", pixels))


socket_file = process.argv[2] || throw "Socket file must be specified"
new RealPiano(socket_file).runLoop()