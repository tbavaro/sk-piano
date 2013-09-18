PhysicalPianoKeys = require("./PhysicalPianoKeys")
TestUtils = require("test/TestUtils")

exports.testPerformance = (test) ->
  pianoKeys = new PhysicalPianoKeys()
  pianoKeys.scan()
  test.done()
