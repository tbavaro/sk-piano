LedLocations = require("sim/LedLocations")
GlowComposer = require("sim/GlowComposer")

modelToMesh = (model, material) ->
  mesh = new THREE.Mesh(model, material)
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

class ViewPort
  constructor: (parentDomElement, strip) ->
    @parentDomElement = parentDomElement
    @strip = strip

    @camera = new THREE.PerspectiveCamera(45, 1, 1, 10000)

    @scene = new THREE.Scene()
    @glowScene = new THREE.Scene()

    # will be replaced when scene gets filled
    @ledColors = null

    @fillScene()
    @addLights()

    @renderer = new THREE.WebGLRenderer({ antialias: true })
    @renderer.autoClear = false

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
        e.preventDefault()
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
        e.preventDefault()
      $(@renderer.domElement).bind "mousewheel", (e) =>
        @radius += e.originalEvent.wheelDelta * 0.1
        @radius = 75 if @radius < 75
        @radius = 300 if @radius > 300
        e.preventDefault()
    )()

    parentDomElement.appendChild(@renderer.domElement)

    @glowComposer = new GlowComposer(@renderer, @camera, @scene, @glowScene)

    # set the size and aspect ratio, and update that if the window ever resizes
    @onResize()
    $(window).bind "resize", @onResize.bind(this)

    @animate()

  fillScene: () ->
    loader = new THREE.JSONLoader()
    loader.load "models/piano.js", (model) =>
      # add full-black piano model to the glow scene
      flatBlackMaterial = new THREE.MeshBasicMaterial({ color: 0x000000 })
      @glowScene.add(modelToMesh(model.clone(), flatBlackMaterial))

      material = new THREE.MeshLambertMaterial({
        color:0x222222
        side: THREE.DoubleSide
        shading: THREE.FlatShading
      })
      mesh = modelToMesh(model, material)
      @scene.add(mesh)

      geometry = new THREE.SphereGeometry(0.2, 4, 4)
      glowLedGeometry = new THREE.SphereGeometry(0.66, 6, 6)

      ledMaterial = new THREE.MeshBasicMaterial({})
      glowLedMaterial = new THREE.MeshBasicMaterial({
        blending:THREE.AdditiveBlending
        transparent:true
      })

      ledLocations = (p.clone().add(mesh.position) for p in LedLocations)
      @ledColors = []
      for p in ledLocations
        material = ledMaterial.clone()
        glowMaterial = glowLedMaterial.clone()
        glowMaterial.color = material.color
        @ledColors.push(material.color)

        led = new THREE.Mesh(geometry, material)
        led.position = p
        @scene.add(led)

        led = new THREE.Mesh(glowLedGeometry, glowMaterial)
        led.position = p
        @glowScene.add(led)
      return

  setLedColors: (colors) ->
    if @ledColors != null
      for color, i in colors
        @ledColors[i].set(color)
    return

  addLights: () ->
    ambientLight = new THREE.AmbientLight(0x090909)
    @scene.add(ambientLight)

    directionalLight = new THREE.DirectionalLight(0x666644)
    directionalLight.position.set(-20, -70, 100).normalize()
    @scene.add(directionalLight)

    directionalLight = new THREE.DirectionalLight(0x444444)
    directionalLight.position.set(60, -70, 100).normalize()
    @scene.add(directionalLight)

    directionalLight = new THREE.DirectionalLight(0x444444)
    directionalLight.position.set(60, 0, -100).normalize()
    @scene.add(directionalLight)
    return

  onResize: () ->
    width = @parentDomElement.clientWidth
    height = @parentDomElement.clientHeight
    @camera.aspect = width / height
    @camera.updateProjectionMatrix()
    @renderer.setSize(width, height)
    @glowComposer.setSize(width, height)
    return

  animate: () ->
    # note: three.js includes requestAnimationFrame shim
    requestAnimationFrame(@animate.bind(this))

    @setLedColors(@strip.colors())
    @render()
    return

  render: () ->
    @camera.position.x = Math.sin(@rotationX) * Math.sin(@rotationY) * @radius
    @camera.position.z = Math.cos(@rotationX) * Math.sin(@rotationY) * @radius
    @camera.position.y = Math.cos(@rotationY) * @radius
    @camera.lookAt(new THREE.Vector3(0, 0, 0))

    @glowComposer.render()

    return


module.exports = ViewPort
