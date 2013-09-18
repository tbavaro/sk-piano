// fix up weird paths in polvo-generated bundle
(function() {
  var prefix = "src/";
  for (var path in require.mods) {
    if (typeof path === "string" && path.indexOf(prefix) === 0) {
      var new_path = path.substring(prefix.length);
      require.mods[new_path] = require.mods[path];
      delete require.mods[path];
    }
  }
})();

function onLoad() {
  var Simulator = require("sim/Simulator");
  window.simulator = new Simulator();
}
