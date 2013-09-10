PhysicalPianoKeys = require("./PhysicalPianoKeys")
TestUtils = require("./TestUtils")

exports.testPerformance = (test) ->
  pianoKeys = new PhysicalPianoKeys()
  pianoKeys.scan()
  test.done()
