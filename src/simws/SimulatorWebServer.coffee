express = require("express")
path = require("path")

PORT = 8380
STATIC_DIR = path.join(__dirname, "../../sim_www")

server = express()
server.use(express.static(STATIC_DIR))

server.listen(PORT)

console.log "Listening on port #{PORT}..."
