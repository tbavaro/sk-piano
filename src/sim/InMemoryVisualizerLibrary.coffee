VisualizerLibrary = require("base/VisualizerLibrary")
assert = require("assert")

KEY_PREFIX = "vis:"

keyify = (name) ->
  assert(VisualizerLibrary.isValidName(name), "invalid name")
  "#{KEY_PREFIX}#{name}"

unkeyify = (key) ->
  key.substring(KEY_PREFIX.length)

module.exports = class InMemoryVisualizerLibrary extends VisualizerLibrary
  constructor: () ->
    @data = {}

  list: () ->
    (unkeyify(name) for name of @data).sort()

  read: (name) ->
    @data[keyify(name)] || null

  write: (name, contents) ->
    @data[keyify(name)] = contents
