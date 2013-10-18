ServerDatastore = require("sim/ServerDatastore")
assert = require("assert")
fs = require("fs")

generateTestDatabaseName = () ->
  "test-" + Math.floor(Math.random() * 1000000)

runWithTestDatabase = (func) ->
  db = new ServerDatastore(generateTestDatabaseName())
  try
    func(db)
  finally
    db.close () => fs.unlink(db.dbFilename)

module.exports =
  testBasic: (test) ->
    runWithTestDatabase (db) ->
      db.createDocument("test", "blah")
      db.getMetadataForAllDocuments (entries) =>
        assert.equal(1, entries.length)
        test.done()
#        console.log("entries: #{JSON.stringify(entries, null, 2)}")

      #db.getCodeForDocument 7, (code) =>
      #  console.log("code: #{code}")

      #db.deleteDocument(1)
