#WebSocketServer = require("ws").Server
#server = new WebSocketServer { port: 8001 }
#server.on("connection", (ws) ->
#  console.log "Connected!"
#
#  ws.on("message", (message) ->
#    console.log("received: %s", message))
#  ws.send("hello"))


Socket = require("net").Socket
socket = new Socket

socket.on("error", (error) ->
  if error.code == "ECONNREFUSED"
    console.log("Unable to connect.", error)
  else
    console.log("Unknown error.", error))

socket.on("connect", () ->
  console.log("Connected!"))

socket.connect("/tmp/simulator.socket")
