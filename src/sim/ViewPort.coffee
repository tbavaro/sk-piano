Shaders = require("sim/webgl/Shaders")

class ViewPort
  constructor: (parentDomElement) ->
    @parentDomElement = parentDomElement
    @camera = new THREE.PerspectiveCamera(45, 1, 1, 10000)

    @scene = new THREE.Scene()

    ambientLight = new THREE.AmbientLight(0xffffff)
    @scene.add(ambientLight)

    directionalLight = new THREE.DirectionalLight(0xffeedd)
    directionalLight.position.set(0, -70, 100).normalize()
    @scene.add(directionalLight)

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

          # don't take focus
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
      $(@renderer.domElement).bind "mousewheel", (e) =>
        console.log(e)
        @radius += e.originalEvent.wheelDelta * 0.1
        @radius = 75 if @radius < 75
        @radius = 300 if @radius > 300
    )()

    # set the size and aspect ratio, and update that if the window ever resizes
    @onResize()
    $(window).bind "resize", @onResize.bind(this)

    parentDomElement.appendChild(@renderer.domElement)

    # piano model
    loader = new THREE.JSONLoader()
    loader.load "models/piano.js", (model) =>
      @mesh = new THREE.Mesh(model, new THREE.MeshNormalMaterial())
      @mesh.material.side = THREE.DoubleSide
      @mesh.geometry.computeBoundingBox()
      box = @mesh.geometry.boundingBox
      @mesh.position.x = -1 * (box.max.x + box.min.x) * 0.5
      @mesh.position.y = -1 * (box.max.y + box.min.y) * 0.45
      @mesh.position.z = -1 * (box.max.z + box.min.z) / 2
      @scene.add(@mesh)
      @camera.position

    # lights
    particleCount = 10000
    particles = new THREE.Geometry()
    pMaterial =
      new THREE.ParticleBasicMaterial({
      color: 0xFFFFFF,
      size: 1
      })

    for p in [0...particleCount] by 1
      pX = Math.random() * 500 - 250
      pY = Math.random() * 500 - 250
      pZ = Math.random() * 500 - 250
      particle = new THREE.Vertex(
        new THREE.Vector3(pX, pY, pZ))

      particles.vertices.push(particle)

    particleSystem =
      new THREE.ParticleSystem(
        particles,
      pMaterial)

    @scene.add(particleSystem);

    console.log(@camera)
    @animate()

  onResize: () ->
    @camera.aspect = @parentDomElement.clientWidth / @parentDomElement.clientHeight
    @camera.updateProjectionMatrix()
    @renderer.setSize(@parentDomElement.clientWidth, @parentDomElement.clientHeight)

  animate: () ->
    # note: three.js includes requestAnimationFrame shim
    requestAnimationFrame(@animate.bind(this))
    @render()

  render: () ->
    @camera.position.x = Math.sin(@rotationX) * Math.sin(@rotationY) * @radius
    @camera.position.z = Math.cos(@rotationX) * Math.sin(@rotationY) * @radius
    @camera.position.y = Math.cos(@rotationY) * @radius
    @camera.lookAt(new THREE.Vector3(0, 0, 0))
#    if @mesh
#      @mesh.rotation.z += 0.01
#      @mesh.rotation.y += 0.02
    @renderer.render(@scene, @camera)


module.exports = ViewPort
