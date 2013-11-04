_bracket01 = (v) -> if v < 0 then 0 else (if v > 1 then 1 else v)

_rgbUnchecked = (r, g, b) -> (g << 16) | (r << 8) | b

_hsvUnchecked = (h, s, v) ->
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
  _rgbUnchecked(r * 127, g * 127, b * 127)

_colorPart = (c, shift) -> (0x7f & (c >> shift))

_redPart = (c) -> _colorPart(c, 8)
_greenPart = (c) -> _colorPart(c, 16)
_bluePart = (c) -> _colorPart(c, 0)

class Colors
  ###
  Utility functions for generating color values.

  In addition to these functions, the constants `Colors.BLACK` and
  `Colors.WHITE` are what you would expect.
  ###

  @rgb:(r, g, b) ->
    ###
    Generates a color value with the given red, green, and blue values, each
    between 0 and 1.
    ###

    _rgbUnchecked(
      Math.floor(_bracket01(r) * 127),
      Math.floor(_bracket01(g) * 127),
      Math.floor(_bracket01(b) * 127))

  @hsv: (h, s, v) ->
    ###
    Generates a color value with the given hue, saturation, and value values.
    Hue is specified in degrees from 0 to 360, while saturation and value are
    between 0 and 1.
    ###
    _hsvUnchecked(h % 360.0, _bracket01(s), _bracket01(v))

#  @red: (c) -> _redPart(c) / 127.0
#  @green: (c) -> _greenPart(c) / 127.0
#  @blue: (c) -> _bluePart(c) / 127.0

  @BLACK: 0
  @WHITE: 0x7f7f7f

module.exports = Colors
