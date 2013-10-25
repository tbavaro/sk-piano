DATASTORE_VERSION = 2

KEY_PREFIX = "v#{DATASTORE_VERSION}:"

DOCUMENT_METADATA_PREFIX = "#{KEY_PREFIX}docMeta:"
DOCUMENT_CONTENT_PREFIX = "#{KEY_PREFIX}docContent:"
LOADED_DOCUMENT_NAME_KEY = "#{KEY_PREFIX}loadedDocumentName"

DEFAULT_DOCUMENT_NAME = "New Visualizer"

String.prototype.startsWith = String.prototype.startsWith || (value) ->
  this.indexOf(value) == 0

forEachEntry = (func) ->
  for i in [0...localStorage.length] by 1
    key = localStorage.key(i)
    value = localStorage[key]
    func(key, value)
  return

forEachEntryWithPrefix = (prefix, func) ->
  forEachEntry (key, value) =>
    if key.startsWith(prefix)
      name = key.substring(prefix.length)
      func(name, value)

forEachDocumentName = (func) ->
  forEachEntryWithPrefix DOCUMENT_METADATA_PREFIX, (name) => func(name)

forEachDocumentMetadata = (func) ->
  forEachEntryWithPrefix DOCUMENT_METADATA_PREFIX, (name, value) =>
    func(name, JSON.parse(value))

documentMetadataKey = (name) => "#{DOCUMENT_METADATA_PREFIX}#{name}"
documentContentKey = (name) => "#{DOCUMENT_CONTENT_PREFIX}#{name}"

storeDocumentMetadata = (name, metadata) ->
  key = documentMetadataKey(name)
  localStorage[key] = JSON.stringify(metadata)
  return

loadDocumentMetadata = (name) ->
  key = documentMetadataKey(name)
  value = localStorage[key]
  result = (if !value then null else JSON.parse(value))
  result

updateDocumentMetadata = (name, updateFunc) ->
  metadata = loadDocumentMetadata(name)
  if metadata == null then throw "document does not exist: #{name}"
  updateFunc(metadata)
  storeDocumentMetadata(name, metadata)
  return

documentExists = (name) -> (localStorage.getItem(documentMetadataKey(name)) != null)

assertDocumentExists = (name) ->
  if !(documentExists(name))
    throw "document does not exist: #{name} #{documentMetadataKey(name)}"

loadDocumentContent = (name) ->
  assertDocumentExists(name)
  localStorage[documentContentKey(name)] || ""

storeDocumentContent = (name, content) ->
  assertDocumentExists(name)
  localStorage[documentContentKey(name)] = content
  return

class DataStore
  documentNames: () ->
    result = []
    forEachDocumentName (name) => (result.push(name))
    result

  _nextDocumentNameWithPrefix: (prefix) ->
    i = 0
    loop
      name = prefix + (if i > 0 then " (#{i})" else "")
      if !documentExists(name) then return name
      ++i

  newDocument: (opt_name_prefix) ->
    name = @_nextDocumentNameWithPrefix(opt_name_prefix || DEFAULT_DOCUMENT_NAME)
    console.log("new document: #{name}")
    storeDocumentMetadata(name, {})
    storeDocumentContent(name, "")
    name

  duplicateDocument: (name) ->
    metadata = loadDocumentMetadata(name)
    content = @documentContent(name)

    newName = @_nextDocumentNameWithPrefix(name)
    storeDocumentMetadata(newName, metadata)
    storeDocumentContent(newName, content)

    newName

  deleteDocument: (name) ->
    assertDocumentExists(name)
    localStorage.removeItem(documentMetadataKey(name))
    localStorage.removeItem(documentContentKey(name))
    return

  documentContent: (name) -> loadDocumentContent(name)

  setDocumentContent: (name, content) -> storeDocumentContent(name, content)

  loadedDocumentName: () ->
    result = localStorage[LOADED_DOCUMENT_NAME_KEY] || null

    # default to the last document created, or null
    if result == null
      forEachDocumentName (name) => (result = name)

    result

  setLoadedDocumentName: (name) ->
    localStorage[LOADED_DOCUMENT_NAME_KEY] = name
    return

module.exports = DataStore
