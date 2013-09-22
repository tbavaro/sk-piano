Shaders = require("sim/webgl/Shaders")

XCXC = null

class ViewPort
  constructor: (parentDomElement) ->
    @parentDomElement = parentDomElement
    @camera = new THREE.PerspectiveCamera(75, 1, 1, 10000)
    @camera.position.z = 200

    @scene = new THREE.Scene()

    ambientLight = new THREE.AmbientLight(0xffffff)
    @scene.add(ambientLight)

    directionalLight = new THREE.DirectionalLight(0xffeedd)
    directionalLight.position.set(0, -70, 100).normalize()
    @scene.add(directionalLight)

    @renderer = new THREE.WebGLRenderer()

    # set the size and aspect ratio, and update that if the window ever resizes
    @onResize()
    window.addEventListener "resize", @onResize.bind(this)

    parentDomElement.appendChild(@renderer.domElement)

    loader = new THREE.JSONLoader()
    loader.load "models/piano.js", (model) =>
      @mesh = new THREE.Mesh(model, new THREE.MeshNormalMaterial())
      @mesh.material.side = THREE.DoubleSide
      @mesh.geometry.computeBoundingBox()
      box = @mesh.geometry.boundingBox
      @mesh.position.x = -1 * (box.max.x + box.min.x) / 2
      @mesh.position.y = -1 * (box.max.y + box.min.y) / 2
      @mesh.position.z = -1 * (box.max.z + box.min.z) / 2
      @scene.add(@mesh)
      @camera.position

    @angle = 0

    console.log(@camera)
    @animate()


#
#    @scene = new THREE.Scene()
#    geometry = new THREE.CubeGeometry(200, 200, 200)
#    material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true })
##    @mesh = new THREE.Mesh(geometry, material)
##    @mesh.scale.x = @mesh.scale.y = @mesh.scale.z = 5
#    console.log(@mesh)
#    @scene.add(@mesh)
#
#    loader = new THREE.ColladaLoader()
#    scene = @scene
#    me = this
#    loader.load "models/piano.dae", (result) ->
#      mesh = result.scene
#      mesh.scale.x = mesh.scale.y = mesh.scale.z = 10
#      me.mesh = mesh
#      scene.add(mesh)
#
##
##    # Add some lights to the scene
##    directionalLight = new THREE.DirectionalLight(0xeeeeee , 1.0)
##    directionalLight.position.x = 1
##    directionalLight.position.y = 0
##    directionalLight.position.z = 0
##    scene.add(directionalLight)
#
#    @renderer = new THREE.CanvasRenderer()
#    @renderer.setClearColorHex(0x00ff00, 1)
#
#
#    parentDomElement.appendChild(@renderer.domElement)
#    @animate()

  onResize: () ->
    @camera.aspect = @parentDomElement.clientWidth / @parentDomElement.clientHeight
    @camera.updateProjectionMatrix()
    @renderer.setSize(@parentDomElement.clientWidth, @parentDomElement.clientHeight)

  animate: () ->
    # note: three.js includes requestAnimationFrame shim
    requestAnimationFrame(@animate.bind(this))
    @render()

  render: () ->
    radius = 100
    @camera.position.x = Math.cos(@angle) * 100
    @camera.position.z = Math.sin(@angle) * 100
    @angle += -0.01
    @camera.lookAt(new THREE.Vector3(0, 0, 0))
#    if @mesh
#      @mesh.rotation.z += 0.01
#      @mesh.rotation.y += 0.02
    @renderer.render(@scene, @camera)


module.exports = ViewPort
module.exports.xcxc = XCXC