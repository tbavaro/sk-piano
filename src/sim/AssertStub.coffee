assert = (condition, optMessage) ->
  throw (optMessage || "assert failed") if (!condition)

assert.deepEqual = () ->
  throw "assert.deepEqual not supported"

module.exports = assert
