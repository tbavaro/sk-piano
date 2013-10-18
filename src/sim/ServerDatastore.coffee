sqlite3 = require("sqlite3")

DATABASE_DIR = "#{__dirname}/../../simdb"
DEFAULT_DATABASE_NAME = "db"

VISUALIZERS_TABLE = "visualizers"
VISUALIZERS_ID = "id"
VISUALIZERS_NAME = "name"
VISUALIZERS_CODE = "code"

INIT_SQL = """
    CREATE TABLE IF NOT EXISTS #{VISUALIZERS_TABLE}
        (#{VISUALIZERS_ID} INTEGER PRIMARY KEY AUTOINCREMENT,
         #{VISUALIZERS_NAME} TEXT UNIQUE NOT NULL,
         #{VISUALIZERS_CODE} TEXT NOT NULL);
"""

class ServerDatastore
  constructor: (opt_database_name) ->
    @dbName = opt_database_name || DEFAULT_DATABASE_NAME
    @dbFilename = "#{DATABASE_DIR}/#{@dbName}"
    @db = new sqlite3.Database(@dbFilename)
    @initialize()

  initialize: () ->
    @db.exec INIT_SQL

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

  deleteDocument: (id, callback) ->
    @db.run """
        DELETE FROM #{VISUALIZERS_TABLE} WHERE #{VISUALIZERS_ID} = ?
    """, id, (error) ->
      if error then throw error
      if this.changes == 0 then throw "no document with id: #{id}"
      if callback
        callback()

  getMetadataForAllDocuments: (callback) ->
    @db.all """
        SELECT
            #{VISUALIZERS_ID},
            #{VISUALIZERS_NAME}
        FROM #{VISUALIZERS_TABLE}
    """, (err, rows) =>
      if err then throw err
      callback [{
        id: row[VISUALIZERS_ID],
        name: row[VISUALIZERS_NAME]
      } for row in rows]

  setNameForDocument: (id, name, callback) ->
    @_setValueForDocument(id, VISUALIZERS_NAME, name, callback)

  getCodeForDocument: (id, callback) ->
    @_getValueForDocument(id, VISUALIZERS_CODE, callback)

  setCodeForDocument: (id, code, callback) ->
    @_setValueForDocument(id, VISUALIZERS_CODE, code, callback)

  _getValueForDocument: (id, field, callback) ->
    @db.get """
        SELECT #{field}
        FROM #{VISUALIZERS_TABLE}
        WHERE #{VISUALIZERS_ID} = ?
    """, id, (err, row) =>
      if err then throw err
      if row == undefined then throw "no document with id: #{id}"
      callback row[field]

  _setValueForDocument: (id, field, value, callback) ->
    @db.run """
        UPDATE #{VISUALIZERS_TABLE}
        SET #{field} = ?
        WHERE #{VISUALIZERS_ID} = ?
    """, value, id, (error) ->
      if error then throw error
      if this.changes == 0 then throw "no document with id: #{id}"
      if callback
        callback()

module.exports = ServerDatastore
