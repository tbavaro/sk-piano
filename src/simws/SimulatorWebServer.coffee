connect = require("connect")
express = require("express")
path = require("path")
ServerVisualizerLibrary = require("base/ServerVisualizerLibrary")

PORT = 8380
STATIC_DIR = path.join(__dirname, "../../sim_www")

class SimulatorWebServer
  constructor: () ->
    @activeLibrary = ServerVisualizerLibrary.activeVisualizers()
    @tutorialLibrary = ServerVisualizerLibrary.tutorialVisualizers()
    @server = express()

    @server.use "/visualizers", (req, res) =>
      code = null

      m = req.path.match(/^\/(.*)\.coffee$/)
      if m != null
        name = m[1]
        code = @tutorialLibrary.read(name)
        if code == null
          code = @activeLibrary.read(name)

      if code == null
        res.send 404, "Not found"
      else
        res.set "Content-Type", "text/coffeescript"
        res.send code

    @server.use(connect.compress())
    @server.use(express.static(STATIC_DIR))

  listen: (port) ->
    @server.listen(port)
    console.log "Listening on port #{port}..."


ws = new SimulatorWebServer()
ws.listen(PORT)
