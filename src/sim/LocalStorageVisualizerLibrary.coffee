VisualizerLibrary = require("base/VisualizerLibrary")

DATASTORE_VERSION = 3

KEY_PREFIX = "v#{DATASTORE_VERSION}:"

DOCUMENT_CONTENT_PREFIX = "#{KEY_PREFIX}docContent:"
LOADED_DOCUMENT_NAME_KEY = "#{KEY_PREFIX}loadedDocumentName"

DEFAULT_DOCUMENT_NAME = "New Visualizer"

String.prototype.startsWith = String.prototype.startsWith || (value) ->
  this.indexOf(value) == 0

forEachKey = (func) ->
  for i in [0...localStorage.length] by 1
    key = localStorage.key(i)
    func(key)
  return

forEachEntry = (func) ->
  for i in [0...localStorage.length] by 1
    key = localStorage.key(i)
    value = localStorage[key]
    func(key, value)
  return

forEachKeyWithPrefix = (prefix, func) ->
  forEachKey (key) =>
    if key.startsWith(prefix)
      name = key.substring(prefix.length)
      func(name)

forEachDocumentName = (func) ->
  forEachKeyWithPrefix DOCUMENT_CONTENT_PREFIX, func

documentContentKey = (name) => "#{DOCUMENT_CONTENT_PREFIX}#{name}"

documentExists = (name) -> (localStorage.getItem(documentContentKey(name)) != null)

assertDocumentExists = (name) ->
  if !(documentExists(name))
    throw "document does not exist: #{name}"

loadDocumentContent = (name) ->
  assertDocumentExists(name)
  localStorage[documentContentKey(name)] || ""

storeDocumentContent = (name, content) ->
  localStorage[documentContentKey(name)] = content
  return

module.exports = class LocalStorageVisualizerLibrary extends VisualizerLibrary
  constructor: () ->
    super

  list: () ->
    result = []
    forEachDocumentName (name) => (result.push(name))
    result

  _nextDocumentNameWithPrefix: (prefix) ->
    i = 0
    loop
      name = prefix + (if i > 0 then " (#{i})" else "")
      if !documentExists(name) then return name
      ++i

  create: (opt_name_prefix) ->
    name = @_nextDocumentNameWithPrefix(opt_name_prefix || DEFAULT_DOCUMENT_NAME)
    console.log("new document: #{name}")
    storeDocumentContent(name, "")
    name

  defaultDuplicateDocumentName: (name) ->
    @_nextDocumentNameWithPrefix(name)

  duplicate: (name, opt_newName) ->
    content = @read(name)
    newName = @_nextDocumentNameWithPrefix(opt_newName || name)
    storeDocumentContent(newName, content)

    newName

  remove: (name) ->
    assertDocumentExists(name)
    localStorage.removeItem(documentContentKey(name))
    return

  read: (name) -> loadDocumentContent(name)

  write: (name, content) -> storeDocumentContent(name, content)

  loadedDocumentName: () ->
    result = localStorage[LOADED_DOCUMENT_NAME_KEY] || null

    # default to the last document created, or null
    if result == null
      forEachDocumentName (name) => (result = name)

    result

  setLoadedDocumentName: (name) ->
    localStorage[LOADED_DOCUMENT_NAME_KEY] = name
    return

  rename: (oldName, desiredNewName) ->
    wasLoadedDocument = (@loadedDocumentName() == oldName)
    newName = @duplicate(oldName, desiredNewName)
    @remove(oldName)
    if wasLoadedDocument then @setLoadedDocumentName(newName)
    newName
