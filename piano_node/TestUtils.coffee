assert = require("assert")

TestUtils =
  runTimed: (func) ->
    startTime = Date.now()
    func()
    (Date.now() - startTime)

  assertMaxRunTime: (maxRunTime, func) ->
    runTime = TestUtils.runTimed(func)
    assert(runTime <= maxRunTime,
        "expected duration <= " + maxRunTime + ", was: " + runTime)

  assertBufferEquals: (expectedArray, buffer) ->
    array = new Array(buffer.length)
    for i in [0...buffer.length] by 1
      array[i] = buffer[i]
    assert.deepEqual(expectedArray, array)

module.exports = TestUtils
