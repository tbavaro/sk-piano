class LedGroup
  constructor: (numLeds, points) ->
    points = (new THREE.Vector3(p[0], p[2], -1 * p[1]) for p in points)

    prevPoint = null
    totalLength = 0
    for point in points
      totalLength += prevPoint.distanceTo(point) if prevPoint != null
      prevPoint = point

    distanceBetweenPoints = totalLength / numLeds

    pointsCopy = points.slice().reverse()
    prevPoint = null
    nextPoint = pointsCopy.pop()
    direction = null
    currentSegmentLength = 0
    distanceAlongCurrentSegment = 0
    advancePoint = () =>
      distanceAlongCurrentSegment -= currentSegmentLength
      prevPoint = nextPoint
      nextPoint = pointsCopy.pop() || null
      if nextPoint == null
        direction = new THREE.Vector3()
        currentSegmentLength = 0
      else
        p = nextPoint.clone()
        direction = p.sub(prevPoint).normalize()
        currentSegmentLength = prevPoint.distanceTo(nextPoint)
    advancePoint()

    @leds = new Array(numLeds)
    for i in [0...numLeds] by 1
      advancePoint() while (
        nextPoint != null && currentSegmentLength < distanceAlongCurrentSegment)
      @leds[i] =
        direction.clone().multiplyScalar(distanceAlongCurrentSegment).add(prevPoint)
      distanceAlongCurrentSegment += distanceBetweenPoints

# (hidden in the middle of the piano)
TODO_UNMAPPED = [
  [ 30, 20, 35 ],
  [ 30, 20, 35.01 ]
]

groups = [
  # LEDS 0-79, top behind hinges
  new LedGroup(80, [
    [-2.500000, 3.622857, 40.629805],
    [-2.500000, 39.529132, 40.629805],
    [0.248869, 48.653022, 42.216865],
    [6.417874, 55.178222, 45.778542],
    [14.529578, 57.541991, 50.461836],
    [22.641282, 55.178222, 55.145131],
    [28.810286, 48.653022, 58.706807],
    [31.559155, 39.529132, 60.293867],
    [32.881504, 32.706831, 61.057326],
    [35.706085, 26.523176, 62.688098],
    [39.835435, 21.410457, 65.072180],
    [44.980877, 17.726097, 68.042902],
    [47.936872, 13.111188, 69.749546],
    [48.984187, 7.500000, 70.354214],
    [48.984187, 3.622857, 70.354214]
  ]),

  # LEDS 80-91, top right in front of hinge
  new LedGroup(12, TODO_UNMAPPED),

  # LEDS 92-135, top front row
  new LedGroup(44, TODO_UNMAPPED),

  # LEDS 136-147, top left in front of hinge
  new LedGroup(12, TODO_UNMAPPED),

  # LEDS 148-162, second row left side up to keys
  new LedGroup(15, [
    [-2.200000, 3.622857, 38.763780],
    [-2.200000, -7.492655, 38.763780],
    [-0.4, -7.492655, 38.763780],
    [-0.4, -5.755446, 37.244905]
  ]),

  # LEDS 163-203, second row above keys
  new LedGroup(41, [
    [0.25, -5.555446, 37.244905],
    [55.848819, -5.555446,  37.244905]
  ]),

  # LEDS 204-218, second row right side keys to hinge
  new LedGroup(15, [
    [55.848819, -5.555446,  37.244905],
    [55.848819, -7.492655, 38.763780],
    [57.648819, -7.492655, 38.763780],
    [57.648819, 3.622857, 38.763780]
  ]),

  # LEDS 219-297, second row right hinge around to left hinge
  new LedGroup(79, [
    [57.648819, 3.622857, 38.763780],
    [57.648819, 7.500000, 38.763780],
    [56.239483, 13.111188, 38.763780],
    [52.826194, 17.726097, 38.763780],
    [46.884750, 21.410457, 38.763780],
    [42.116587, 26.523176, 38.763780],
    [38.855042, 32.706831, 38.763780],
    [37.328125, 39.529132, 38.763780],
    [34.154004, 48.653022, 38.763780],
    [27.030652, 55.178222, 38.763780],
    [17.664063, 57.541991, 38.763780],
    [8.297473, 55.178222, 38.763780],
    [1.174121, 48.653022, 38.763780],
    [-2.200000, 39.529132, 38.763780],
    [-2.200000, 3.622857, 38.763780]
  ]),

  # LEDS 298-319, third row left up to keys
  new LedGroup(22, [
    [-2.200000, 7.500000, 26.500000],
    [-2.200000, -15.492655, 26.500000]
  ]),

  # LEDS 320-363, third row across the front
  new LedGroup(44, [
    [-2.200000, -15.492655, 26.500000],
    [57.648819, -15.492655, 26.500000]
  ]),

  # LEDS 364-457, third row front right all the way around to left hinge
  new LedGroup(94, [
    [57.648819, -15.492655, 26.500000],
    [57.648819, 7.500000, 26.500000],
    [56.239483, 13.111188, 26.500000],
    [52.826194, 17.726097, 26.500000],
    [46.884750, 21.410457, 26.500000],
    [42.116587, 26.523176, 26.500000],
    [38.855042, 32.706831, 26.500000],
    [37.328125, 39.529132, 26.500000],
    [34.154004, 48.653022, 26.500000],
    [27.030652, 55.178222, 26.500000],
    [17.664063, 57.541991, 26.500000],
    [8.297473, 55.178222, 26.500000],
    [1.174121, 48.653022, 26.500000],
    [-2.200000, 39.529132, 26.500000],
    [-2.200000, 7.500000, 26.500000]
  ]),

  # LEDS 458-477, bottom row left up to keys
  new LedGroup(22, [
    [-2.200000, 7.500000, 24.000000],
    [-2.200000, -15.492655, 24.000000]
  ]),

  # LEDS 478-521, bottom row across the front
  new LedGroup(44, [
    [-2.200000, -15.492655, 24.000000],
    [57.648819, -15.492655, 24.000000]
  ]),

  # LEDS 522-541, third row front right all the way around to left hinge
  new LedGroup(20, [
    [57.648819, -15.492655, 24.000000],
    [57.648819, 7.500000, 24.000000],
  ])
]

module.exports = [].concat.apply([], group.leds for group in groups)
