spawn = require("child_process").spawn
fs = require("fs")

dirsAndFilePatterns =
  ".": ["Makefile"]
  "src/sim": [/\.(coffee|scss)$/]
  "src/simws": [/\.coffee$/]
  "src/base": [/\.coffee$/]
  "src/lib": [/\.coffee$/]
  "src/sim/webgl": [/\.(coffee|vert|frag)$/, "compile_shaders.rb"]
  "src/visualizers": [/\.coffee$/]

MAKE_CMD = ["make", "sim-force"]

delayMs = 500

pendingTimeout = null
isMaking = false

remake = () ->
  # if we're currently running make, mark ourselves dirty and check back later
  if isMaking
    markDirty()
    return

  console.log("** Remaking...")
  isMaking = true
  child = spawn(MAKE_CMD[0], MAKE_CMD[1..], { stdio: "inherit" })
  child.on "exit", (code, signal) ->
    console.log "** Make completed with exit code: #{code}"
    isMaking = false

markDirty = () ->
  clearTimeout(pendingTimeout) if pendingTimeout != null
  pendingTimeout = setTimeout(remake, delayMs)

matchesPattern = (pattern, string) ->
  if typeof pattern == "string"
    pattern == string
  else
    pattern.exec(string) != null

makePatternMatcher = (string) -> (pattern) -> matchesPattern(pattern, string)

makeDirListener = (dir, filePatterns) ->
  (event, filename) ->
    patternMatcher = makePatternMatcher(filename)
    if filePatterns.some patternMatcher
      console.log "** File was modified: #{dir}/#{filename}"
      markDirty()

for dir, patterns of dirsAndFilePatterns
  console.log "watching #{dir}"
  fs.watch(dir, makeDirListener(dir, patterns))

remake()
