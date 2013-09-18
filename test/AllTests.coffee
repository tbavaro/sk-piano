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

for testSuite in findFilesRecursively(".", /.*Tests\.coffee$/)
  testSuiteName = testSuite.replace(/^.*\//, "")
  exports[testSuiteName] = require("../" + testSuite)
