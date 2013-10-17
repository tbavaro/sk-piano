express = require("express")
path = require("path")

PORT = 8380
STATIC_DIR = path.join(__dirname, "../../sim_www")
VISUALIZERS_DIR = path.join(__dirname, "../visualizers")

server = express()
server.use("/visualizers", express.static(VISUALIZERS_DIR))
server.use(express.static(STATIC_DIR))

server.listen(PORT)

console.log "Listening on port #{PORT}..."
