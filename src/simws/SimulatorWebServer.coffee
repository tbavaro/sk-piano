connect = require("connect")
express = require("express")
path = require("path")
ServerVisualizerLibrary = require("base/ServerVisualizerLibrary")

PORT = 80
STATIC_DIR = path.join(__dirname, "../../sim_www")

dumpVisualizers = (library) =>
  result = {}
  result[name] = library.read(name) for name in library.list()
  result

class SimulatorWebServer
  constructor: () ->
    @activeLibrary = ServerVisualizerLibrary.activeVisualizers()
    @tutorialLibrary = ServerVisualizerLibrary.tutorialVisualizers()
    @server = express()

    @server.use(connect.compress())

    @server.get "/visualizers", (req, res) =>
      content = {}

      content.activeVisualizers = dumpVisualizers(@activeLibrary)
      content.tutorialVisualizers = dumpVisualizers(@tutorialLibrary)

      res.set "Content-Type", "application/json"
      res.send JSON.stringify(content, null, 2)

    @server.use(express.static(STATIC_DIR))

  listen: (port) ->
    @server.listen(port)
    console.log "Listening on port #{port}..."


ws = new SimulatorWebServer()
ws.listen(PORT)
