bracket01 = (v) -> if v < 0 then 0 else (if v > 1 then 1 else v)

rgbUnchecked = (r, g, b) -> (g << 16) | (r << 8) | b

hsvUnchecked = (h, s, v) ->
  if s == 0
    # achromatic (grey)
    r = g = b = v
  else
    h /= 60  #  sector 0 to 5
    i = Math.floor(h)
    f = h - i  # factorial part of h
    p = v * (1 - s)
    q = v * (1 - s * f)
    t = v * (1 - s * (1 - f))
    switch i
      when 0
        r = v
        g = t
        b = p
      when 1
        r = q
        g = v
        b = p
      when 2
        r = p
        g = v
        b = t
      when 3
        r = p
        g = q
        b = v
      when 4
        r = t
        g = p
        b = v
      else  # when 5
        r = v
        g = p
        b = q
  rgbUnchecked(r * 127, g * 127, b * 127)

colorPart = (c, shift) -> (0x7f & (c >> shift))

redPart = (c) -> colorPart(c, 8)
greenPart = (c) -> colorPart(c, 16)
bluePart = (c) -> colorPart(c, 0)

class Colors
  constructor: () -> throw "static only"

  @rgb: (r, g, b) ->
    rgbUnchecked(
      Math.floor(bracket01(r) * 127),
      Math.floor(bracket01(g) * 127),
      Math.floor(bracket01(b) * 127))

  @hsv: (h, s, v) ->
    hsvUnchecked(h % 360.0, bracket01(s), bracket01(v))

  @toHex: (c) ->
    res = c.toString(16)
    "000000".substr(0, if res.length < 6 then 6 - res.length else 0) + res

  @red: (c) -> redPart(c) / 127.0
  @green: (c) -> greenPart(c) / 127.0
  @blue: (c) -> bluePart(c) / 127.0

  @BLACK: 0
  @WHITE: 0x7f7f7f

#perfTest = () ->
#  iterations = 100000
#  n = iterations
#  start_time = (new Date()).getTime()
#  x = 0
#  xr = 0
#  xg = 0
#  xb = 0
#  while (n > 0)
#    h = Math.random() * 360.0
#    s = Math.random()
#    v = Math.random()
#    c = Colors.hsv(h, s, v)
#    r = Colors.red(c)
#    g = Colors.green(c)
#    b = Colors.blue(c)
#    x = c if (c > x)
#    xr = r if (r > xr)
#    xg = g if (g > xg)
#    xb = b if (b > xb)
#    n -= 1
#  end_time = (new Date()).getTime()
#  duration_ms = (end_time - start_time)
#  console.log("total time: " + duration_ms + "ms")
#  console.log(
#    "time per iteration: " + (duration_ms / iterations).toFixed(6) + "ms")
#  console.log(x, xr, xg, xb)
#
#perfTest()
#console.log("rgb: " + Colors.toHex(Colors.rgb(0.5, 0, 0)))

module.exports = Colors
