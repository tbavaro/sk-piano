DOCUMENT_METADATA_PREFIX = "docMeta:"
DOCUMENT_CONTENT_PREFIX = "docContent:"
LOADED_DOCUMENT_ID_KEY = "loadedDocumentId"

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
      id = parseInt(key.substring(prefix.length), 10)
      func(id, value)

forEachDocumentId = (func) ->
  forEachEntryWithPrefix DOCUMENT_METADATA_PREFIX, (id) => func(id)

forEachDocumentMetadata = (func) ->
  forEachEntryWithPrefix DOCUMENT_METADATA_PREFIX, (id, value) =>
    func(id, JSON.parse(value))

documentMetadataKey = (id) => "#{DOCUMENT_METADATA_PREFIX}#{id}"
documentContentKey = (id) => "#{DOCUMENT_CONTENT_PREFIX}#{id}"

storeDocumentMetadata = (id, metadata) ->
  key = documentMetadataKey(id)
  localStorage[key] = JSON.stringify(metadata)
  return

loadDocumentMetadata = (id) ->
  key = documentMetadataKey(id)
  value = localStorage[key]
  result = (if !value then null else JSON.parse(value))
  result

updateDocumentMetadata = (id, updateFunc) ->
  metadata = loadDocumentMetadata(id)
  if metadata == null then throw "document does not exist: #{id}"
  updateFunc(metadata)
  storeDocumentMetadata(id, metadata)
  return

documentExists = (id) -> !!(localStorage[documentMetadataKey(id)])

assertDocumentExists = (id) ->
  if !(documentExists(id))
    throw "document does not exist: #{id} #{documentMetadataKey(id)}"

loadDocumentContent = (id) ->
  assertDocumentExists(id)
  localStorage[documentContentKey(id)] || ""

storeDocumentContent = (id, content) ->
  assertDocumentExists(id)
  localStorage[documentContentKey(id)] = content
  return

class DataStore
  constructor: () ->
    @_updateNextId()

  _updateNextId: () ->
    maxId = -1
    forEachDocumentId (id) => (if id > maxId then maxId = id)
    @nextId = (maxId + 1)
    return

  documentNames: () ->
    result = {}
    forEachDocumentMetadata (id, metadata) => (result[id] = metadata.name)
    result

  newDocument: () ->
    id = (@nextId++)
    storeDocumentMetadata(id, { name: DEFAULT_DOCUMENT_NAME })
    @setDocumentContent(id, "")
    id

  duplicateDocument: (id) ->
    metadata = loadDocumentMetadata(id)
    content = @documentContent(id)

    newId = @newDocument()
    storeDocumentMetadata(newId, metadata)
    @setDocumentContent(newId, content)

    newId

  deleteDocument: (id) ->
    assertDocumentExists(id)
    localStorage.removeItem(documentMetadataKey(id))
    localStorage.removeItem(documentContentKey(id))
    return

  documentName: (id) ->
    metadata = loadDocumentMetadata(id)
    if metadata == null then throw "no such document: #{id}"
    metadata.name

  setDocumentName: (id, name) ->
    updateDocumentMetadata id, (metadata) => (metadata.name = name)

  documentContent: (id) -> loadDocumentContent(id)

  setDocumentContent: (id, content) -> storeDocumentContent(id, content)

  loadedDocumentId: () ->
    result = localStorage[LOADED_DOCUMENT_ID_KEY]

    # default to the last document created, or null
    if !result && result != 0
      result = null
      forEachDocumentId (id) => (result = id)

    result

  setLoadedDocumentId: (id) ->
    localStorage[LOADED_DOCUMENT_ID_KEY] = id
    return

module.exports = DataStore
