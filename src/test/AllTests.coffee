fs = require("fs")

findFilesRecursively = (rootPath, pattern) ->
  results = []
  for filename in fs.readdirSync(rootPath)
    path = "#{rootPath}/#{filename}"
    stats = fs.statSync(path)
    if stats.isDirectory()
      results = results.concat(findFilesRecursively(path, pattern))
    else if pattern.exec(filename) and filename != "AllTests.coffee"
      results.push(path)
  results

originalDir = process.cwd()
process.chdir(__dirname)
testSuiteSources = findFilesRecursively("..", /.*Tests\.coffee$/)
process.chdir(originalDir)

for testSuiteSource in testSuiteSources
  testSuiteName = testSuiteSource.replace(/^.*\//, "")
  exports[testSuiteName] = require(testSuiteSource)
