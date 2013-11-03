connect = require("connect")
express = require("express")
path = require("path")
ServerDatastore = require("sim/ServerDatastore")

PORT = 8380
STATIC_DIR = path.join(__dirname, "../../sim_www")

class SimulatorWebServer
  constructor: () ->
    @datastore = new ServerDatastore()
    @server = express()

    @server.use "/visualizers", (req, res) =>
      returnCodeOrNull = (code) =>
        if code == null
          res.send 404, "Not found"
        else
          res.set "Content-Type", "text/coffeescript"
          res.send code

      m = req.path.match(/^\/(.*)\.coffee$/)
      if m == null
        returnCodeOrNull(null)
      else
        @datastore.getCodeForDocumentOrNull m[1], returnCodeOrNull

    @server.use(connect.compress())
    @server.use(express.static(STATIC_DIR))

  listen: (port) ->
    @server.listen(port)
    console.log "Listening on port #{port}..."


ws = new SimulatorWebServer()
ws.listen(PORT)
