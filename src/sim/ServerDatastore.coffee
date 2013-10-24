sqlite3 = require("sqlite3")
fs = require("fs")

DATABASE_DIR = "#{__dirname}/../../simdb"
DEFAULT_DATABASE_NAME = "db"

DEFAULT_VISUALIZERS_DIR = "#{__dirname}/../visualizers"

VISUALIZERS_TABLE = "visualizers"
VISUALIZERS_NAME = "name"
VISUALIZERS_CODE = "code"

INIT_SQL = """
    CREATE TABLE IF NOT EXISTS #{VISUALIZERS_TABLE}
        (#{VISUALIZERS_NAME} TEXT PRIMARY KEY NOT NULL,
         #{VISUALIZERS_CODE} TEXT NOT NULL);
"""

class ServerDatastore
  constructor: (opt_database_name) ->
    @dbName = opt_database_name || DEFAULT_DATABASE_NAME
    @dbFilename = "#{DATABASE_DIR}/#{@dbName}"
    isNew = !fs.existsSync(@dbFilename)
    @db = new sqlite3.Database(@dbFilename)
    if isNew then @initialize()

  initialize: () ->
    @db.exec INIT_SQL
    for visualizer in fs.readdirSync(DEFAULT_VISUALIZERS_DIR)
      m = visualizer.match(/^(.*)\.coffee$/)
      if m != null
        name = m[1]
        codeData = fs.readFileSync("#{DEFAULT_VISUALIZERS_DIR}/#{visualizer}")
        @createDocument(name, codeData.toString())

  close: (callback) ->
    @db.close(callback)

  # if successful, will pass the id to 'callback'
  createDocument: (name, code, callback) ->
    @db.run """
        INSERT INTO #{VISUALIZERS_TABLE}
        (#{VISUALIZERS_NAME}, #{VISUALIZERS_CODE})
        VALUES (?, ?)
    """, name, code, (error) ->
      if error then throw error
      if callback
        callback(this.lastID)

  deleteDocument: (name, callback) ->
    @db.run """
        DELETE FROM #{VISUALIZERS_TABLE} WHERE #{VISUALIZERS_NAME} = ?
    """, name, (error) ->
      if error then throw error
      if this.changes == 0 then throw "no document with name: #{name}"
      if callback
        callback()

  getMetadataForAllDocuments: (callback) ->
    @db.all """
        SELECT #{VISUALIZERS_NAME} FROM #{VISUALIZERS_TABLE}
    """, (err, rows) =>
      if err then throw err
      callback [{
        name: row[VISUALIZERS_NAME]
      } for row in rows]

  getCodeForDocumentOrNull: (name, callback) ->
    @_getValueForDocumentOrNull(name, VISUALIZERS_CODE, callback)

  setCodeForDocument: (name, code, callback) ->
    @_setValueForDocument(name, VISUALIZERS_CODE, code, callback)

  _getValueForDocument: (name, field, callback) ->
    @_getValueForDocumentOrNull name, field, (result) =>
      if result == null then throw "no document with name: #{name}"
      callback result

  _getValueForDocumentOrNull: (name, field, callback) ->
    @db.get """
            SELECT #{field}
            FROM #{VISUALIZERS_TABLE}
            WHERE #{VISUALIZERS_NAME} = ?
            """, name, (err, row) =>
      if err then throw err
      callback (if row == undefined then null else row[field])

  _setValueForDocument: (name, field, value, callback) ->
    @db.run """
        UPDATE #{VISUALIZERS_TABLE}
        SET #{field} = ?
        WHERE #{VISUALIZERS_NAME} = ?
    """, value, name, (error) ->
      if error then throw error
      if this.changes == 0 then throw "no document with name: #{name}"
      if callback
        callback()

module.exports = ServerDatastore
