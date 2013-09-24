LedLocations = require("sim/LedLocations")

modelToMesh = (model) ->
  material =
    new THREE.MeshLambertMaterial(
      {color:0x221711, side:THREE.DoubleSide})
  mesh = new THREE.Mesh(model, material)
  mesh.material.shading = THREE.FlatShading
  mesh.geometry.computeBoundingBox()
  mesh.geometry.mergeVertices()
  mesh.geometry.computeCentroids()
  mesh.geometry.computeFaceNormals()
  mesh.geometry.computeVertexNormals()
  box = mesh.geometry.boundingBox
  mesh.position.x = -1 * (box.max.x + box.min.x) * 0.5
  mesh.position.y = -1 * (box.max.y + box.min.y) * 0.45
  mesh.position.z = -1 * (box.max.z + box.min.z) / 2
  mesh

class LedRenderer
  setColors: (colors) -> throw "abstract"

class ParticleLedRenderer extends LedRenderer
  constructor: (scene, locations) ->
    super

    particles = new THREE.Geometry()
    pMaterial =
      new THREE.ParticleBasicMaterial({
        vertexColors: true
        map: THREE.ImageUtils.loadTexture("textures/led.png")
        transparent: true
        size: 1.25
        blending: THREE.AdditiveBlending
      })

    for p in locations
      particle = new THREE.Vertex(p)
      particles.vertices.push(particle)
      c = new THREE.Color()
      c.setHSL(Math.random(), 1.0, 0.5)
      particles.colors.push(c)

    @particleSystem = new THREE.ParticleSystem(particles, pMaterial)
    scene.add(@particleSystem)

  setColors: (colors) ->
    @particleSystem.geometry.colors = colors
    @particleSystem.geometry.colorsNeedUpdate = true

class SphereLedRenderer extends LedRenderer
  constructor: (scene, locations) ->
    super

    makeLed = (p) ->
      material = new THREE.MeshBasicMaterial({
        color: 0xCC0000
      })

      led = new THREE.Mesh(new THREE.SphereGeometry(0.125, 6, 6), material)
      led.position = p
      led

    @leds = (makeLed(p) for p in locations)
    scene.add(led) for led in @leds

  setColors: (colors) ->
    for i in [0...(colors.length)] by 1
      @leds[i].material.color = colors[i]

class SphereWithGlowLedRenderer extends LedRenderer
  constructor: (scene, locations) ->
    super

    makeLed = (p) ->
      material = new THREE.MeshBasicMaterial({
      color: 0xCC0000
      })

      led = new THREE.Mesh(new THREE.SphereGeometry(0.125, 4, 4), material)
      led.position = p
      led

    @leds = (makeLed(p) for p in locations)
    scene.add(led) for led in @leds

    particles = new THREE.Geometry()
    pMaterial =
      new THREE.ParticleBasicMaterial({
      vertexColors: true
      map: THREE.ImageUtils.loadTexture("textures/glow.png")
      transparent: true
      size: 3
      blending: THREE.AdditiveBlending
      })

    for p in locations
      particle = new THREE.Vertex(p)
      particles.vertices.push(particle)
      c = new THREE.Color()
      c.setHSL(Math.random(), 1.0, 0.5)
      particles.colors.push(c)

    @particleSystem = new THREE.ParticleSystem(particles, pMaterial)
    scene.add(@particleSystem)

  setColors: (colors) ->
    for i in [0...(colors.length)] by 1
      @leds[i].material.color = colors[i]
    @particleSystem.geometry.colors = colors
    @particleSystem.geometry.colorsNeedUpdate = true

class ViewPort
  constructor: (parentDomElement, strip) ->
    @parentDomElement = parentDomElement
    @strip = strip

    @camera = new THREE.PerspectiveCamera(45, 1, 1, 10000)

    @scene = new THREE.Scene()

    # will be replaced when scene gets filled
    @leds = null

    @fillScene()
    @addLights()

    @renderer = new THREE.WebGLRenderer({ antialias: true })

    @rotationX = Math.PI * 0.15
    @rotationY = Math.PI * 0.4
    @radius = 125
    (() =>
      buttonDown = false
      prevX = null
      prevY = null
      $(@renderer.domElement).bind "mousedown", (e) =>
        buttonDown = (e.which == 1)
        if e.which == 1
          prevX = e.offsetX
          prevY = e.offsetY
      $(window).bind "mouseup", (e) =>
        buttonDown = false if e.which == 1
      $(window).bind "mousemove", (e) =>
        return if !buttonDown
        deltaX = e.offsetX - prevX
        deltaY = e.offsetY - prevY
        prevX = e.offsetX
        prevY = e.offsetY
        @rotationX -= (deltaX * 0.01)
        @rotationY -= (deltaY * 0.01)
        @rotationY = 0.001 if @rotationY < 0.001
        @rotationY = Math.PI - 0.001 if @rotationY > (Math.PI - 0.001)
      $(@renderer.domElement).bind "mousewheel", (e) =>
        @radius += e.originalEvent.wheelDelta * 0.1
        @radius = 75 if @radius < 75
        @radius = 300 if @radius > 300
        e.preventDefault()
    )()

    # set the size and aspect ratio, and update that if the window ever resizes
    @onResize()
    $(window).bind "resize", @onResize.bind(this)

    parentDomElement.appendChild(@renderer.domElement)

    @animate()

  fillScene: () ->
    loader = new THREE.JSONLoader()
    loader.load "models/piano.js", (model) =>
      mesh = modelToMesh(model)
      @scene.add(mesh)

      led_locations = (p.clone().add(mesh.position) for p in LedLocations)
      @leds = new SphereWithGlowLedRenderer(@scene, led_locations)

  addLights: () ->
    ambientLight = new THREE.AmbientLight(0x111111)
    @scene.add(ambientLight)

    directionalLight = new THREE.DirectionalLight(0x888888)
    directionalLight.position.set(-20, -70, 100).normalize()
    @scene.add(directionalLight)

    directionalLight = new THREE.DirectionalLight(0x555555)
    directionalLight.position.set(60, -70, 100).normalize()
    @scene.add(directionalLight)

    directionalLight = new THREE.DirectionalLight(0x222222)
    directionalLight.position.set(60, 0, -100).normalize()
    @scene.add(directionalLight)

  onResize: () ->
    @camera.aspect = @parentDomElement.clientWidth / @parentDomElement.clientHeight
    @camera.updateProjectionMatrix()
    @renderer.setSize(@parentDomElement.clientWidth, @parentDomElement.clientHeight)

  animate: () ->
    # note: three.js includes requestAnimationFrame shim
    requestAnimationFrame(@animate.bind(this))

    @leds.setColors(@strip.colors()) if @leds != null
    @render()

  render: () ->
    @camera.position.x = Math.sin(@rotationX) * Math.sin(@rotationY) * @radius
    @camera.position.z = Math.cos(@rotationX) * Math.sin(@rotationY) * @radius
    @camera.position.y = Math.cos(@rotationY) * @radius
    @camera.lookAt(new THREE.Vector3(0, 0, 0))
    @renderer.render(@scene, @camera)


module.exports = ViewPort
