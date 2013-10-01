createGlowComposer = \
    (renderer, camera, glowScene, renderTargetParameters, width, height) ->
  glowRenderTarget =
    new THREE.WebGLRenderTarget(width, height, renderTargetParameters)

  hblur = new THREE.ShaderPass(THREE.HorizontalBlurShader)
  vblur = new THREE.ShaderPass(THREE.VerticalBlurShader)

  blurriness = 2.0
  hblur.uniforms.h.value = blurriness / width
  vblur.uniforms.v.value = blurriness / height

  renderModelGlow = new THREE.RenderPass(glowScene, camera)

  glowComposer = new THREE.EffectComposer(renderer, glowRenderTarget)
  glowComposer.addPass(renderModelGlow)
  glowComposer.addPass(hblur)
  glowComposer.addPass(vblur)

  glowComposer

createFinalShader = (glowComposer) ->
  uniforms:
    # the base scene buffer
    tDiffuse:
      type: "t"
      texture: null

    # the glow scene buffer
    tGlow:
      type: "t"
      value: glowComposer.renderTarget2

  vertexShader: [
    "varying vec2 vUv;"
    "void main() {"
      "vUv = vec2( uv.x, uv.y );"
      "gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );"
    "}"
  ].join("\n")

  fragmentShader: [
    "uniform sampler2D tDiffuse;"
    "uniform sampler2D tGlow;"
    "varying vec2 vUv;"
    "void main() {"
      "vec4 texel = texture2D( tDiffuse, vUv );"
      "vec4 glow = texture2D( tGlow, vUv );"
      "gl_FragColor = texel + (glow * glow) * 1.5;"
    "}"
  ].join("\n")

class GlowComposer
  constructor: (renderer, camera, scene, glowScene) ->
    @renderer = renderer
    @camera = camera
    @scene = scene
    @glowScene = glowScene

  setSize: (width, height) ->
    renderTargetParameters =
      minFilter: THREE.LinearFilter
      magFilter: THREE.LinearFilter
      format: THREE.RGBFormat
      stencilBuffer: false

    @glowComposer =
        createGlowComposer(
            @renderer, @camera, @glowScene, renderTargetParameters,
            width, height)

    finalShader = createFinalShader(@glowComposer)

    # Prepare the base scene render pass
    renderModel = new THREE.RenderPass(@scene, @camera)

    # Prepare the additive blending pass
    finalPass = new THREE.ShaderPass(finalShader)
    finalPass.needsSwap = true

    # Make sure the additive blending is rendered to the screen (since it's the last pass)
    finalPass.renderToScreen = true

    # Prepare the composer's render target
    renderTarget =
        new THREE.WebGLRenderTarget(width, height, renderTargetParameters)

    # Create the composer
    @finalComposer = new THREE.EffectComposer(@renderer, renderTarget)

    # Add all passes
    @finalComposer.addPass(renderModel)
    @finalComposer.addPass(finalPass)

  render: () ->
    @glowComposer.render()
    @finalComposer.render()

module.exports = GlowComposer
