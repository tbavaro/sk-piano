CompiledShaders = require("sim/webgl/CompiledShaders")

Shaders =
  initShader: (gl, name) ->
    shaderConfig = CompiledShaders[name]
    throw "invalid shader name: #{name}" if !shaderConfig

    shader = gl.createShader(gl[shaderConfig.type])
    gl.shaderSource(shader, shaderConfig.source)
    gl.compileShader(shader)

    if !gl.getShaderParameter(shader, gl.COMPILE_STATUS)
      throw gl.getShaderInfoLog(shader)

    shader

module.exports = Shaders
