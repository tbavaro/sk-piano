PhysicalPianoKeys = require("./PhysicalPianoKeys")
TestUtils = require("./TestUtils")

exports.testPerformance = (test) ->
  pianoKeys = new PhysicalPianoKeys()
  test.done()
