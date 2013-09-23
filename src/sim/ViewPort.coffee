LedLocations = require("sim/LedLocations")

modelToMesh = (model) ->
  material =
    new THREE.MeshLambertMaterial(
      {color:0x665522, side:THREE.DoubleSide})
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

fillScene = (scene) ->
  loader = new THREE.JSONLoader()
  loader.load "models/piano.js", (model) =>
    mesh = modelToMesh(model)
    scene.add(mesh)
  
    # lights
    particles = new THREE.Geometry()
    pMaterial =
      new THREE.ParticleBasicMaterial({
        color: 0xFFFFFF,
        size: 1
      })

    particleAdjustment = mesh.position
    for p in LedLocations by 1
      p = p.clone().add(particleAdjustment)
      particle = new THREE.Vertex(p)
      particles.vertices.push(particle)

    particleSystem =
      new THREE.ParticleSystem(
        particles,
      pMaterial)

    scene.add(particleSystem)

class ViewPort
  constructor: (parentDomElement) ->
    @parentDomElement = parentDomElement
    @camera = new THREE.PerspectiveCamera(45, 1, 1, 10000)

    @scene = new THREE.Scene()

    ambientLight = new THREE.AmbientLight(0x333333)
    @scene.add(ambientLight)

    directionalLight = new THREE.DirectionalLight(0xaaaaaa)
    directionalLight.position.set(-20, -70, 100).normalize()
    @scene.add(directionalLight)

    directionalLight = new THREE.DirectionalLight(0x888888)
    directionalLight.position.set(60, -70, 100).normalize()
    @scene.add(directionalLight)

    directionalLight = new THREE.DirectionalLight(0x444444)
    directionalLight.position.set(60, 0, -100).normalize()
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

    fillScene(@scene)

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
    @renderer.render(@scene, @camera)


module.exports = ViewPort
