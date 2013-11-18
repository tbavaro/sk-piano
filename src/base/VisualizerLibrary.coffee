MIN_NAME_LENGTH = 1
MAX_NAME_LENGTH = 128
VALID_NAME_REGEX = /^[A-Za-z0-9_ -'\.#]*$/

module.exports = class VisualizerLibrary
  constructor: () ->

  list: () -> throw "abstract"

  read: (name) -> throw "abstract"

  write: (name, contents) -> throw "abstract"

  @isValidName: (name) ->
    (name.length >= MIN_NAME_LENGTH &&
     name.length <= MAX_NAME_LENGTH &&
     name.match(VALID_NAME_REGEX) != null)
