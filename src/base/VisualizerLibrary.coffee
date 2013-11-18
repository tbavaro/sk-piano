assert = require("assert")
fs = require("fs")

VISUALIZERS_ROOT = "#{__dirname}/../../data/visualizers"
ACTIVE_VISUALIZERS_DIR = "#{VISUALIZERS_ROOT}/active"
TUTORIAL_VISUALIZERS_DIR = "#{VISUALIZERS_ROOT}/tutorial"

METADATA_FILE_NAME = "metadata.json"

VALID_NAME_REGEX = /^[A-Za-z0-9_ -'\.#]*$/

module.exports = class VisualizerLibrary
  @activeVisualizers: () ->
    new VisualizerLibrary(ACTIVE_VISUALIZERS_DIR)

  @tutorialVisualizers: () ->
    new VisualizerLibrary(TUTORIAL_VISUALIZERS_DIR)

  constructor: (rootDir) ->
    assert(rootDir, "root dir must be specified")
    assert(fs.existsSync(rootDir), "root dir does not exist")
    @rootDir = rootDir
    @metadataFile = "#{rootDir}/#{METADATA_FILE_NAME}"
    @watcher = null
    if !fs.existsSync(@metadataFile)
      @_initMetadata()

  list: () ->
    results = []
    for filename in fs.readdirSync(@rootDir)
      m = filename.match(/^(.*)\.coffee$/)
      if m != null
        results.push(m[1])
    results

  read: (name) ->
    filename = @_filename(name)
    if !fs.existsSync(filename)
      null
    else
      fs.readFileSync(filename).toString()

  write: (name, contents) ->
    filename = @_filename(name)
    fs.writeFileSync(filename, contents)

  _filename: (name) ->
    assert(VisualizerLibrary.isValidName(name), "invalid name: #{name}")
    "#{@rootDir}/#{name}\.coffee"

  _readMetadata: () ->
    JSON.parse(fs.readFileSync(@metadataFile))

  _writeMetadata: (metadata) ->
    fs.writeFileSync(@metadataFile, JSON.stringify(metadata))

  _initMetadata: () ->
    @_writeMetadata({ "revision": 0 })

  # @param {Function<void, metadata>} updates metadata in-place
  _updateMetadata: (callback) ->
    metadata = @_readMetadata()
    callback(metadata)
    metadata.revision = (metadata.revision + 1)
    @_writeMetadata(metadata)

  _touch: () ->
    # this will just increment the revision
    @_updateMetadata (() -> null)

  watch: (callback) ->
    assert(@watcher == null, "there's already a watcher")
    @watcher = fs.watch @metadataFile, { persistent: false }, () =>
      callback()

  @isValidName: (name) ->
    name.length > 0 && name.length < 128 && name.match(VALID_NAME_REGEX) != null
