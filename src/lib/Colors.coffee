# @private
bracket01 = (v) -> if v < 0 then 0 else (if v > 1 then 1 else v)

# @private
rgbUnchecked = (r, g, b) -> (g << 16) | (r << 8) | b

# @private
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

#colorPart = (c, shift) -> (0x7f & (c >> shift))

#redPart = (c) -> colorPart(c, 8)
#greenPart = (c) -> colorPart(c, 16)
#bluePart = (c) -> colorPart(c, 0)

# Utility functions for generating and manipulating color values.
# @author tbavaro
module.exports = class Colors
  # Generates a color value with the given red, green, and blue values.
  # @param {float} red between 0 and 1
  # @param {float} green between 0 and 1
  # @param {float} blue between 0 and 1
  # @return {Color}
  @rgb:(red, green, blue) ->
    rgbUnchecked(
      Math.floor(bracket01(red) * 127),
      Math.floor(bracket01(green) * 127),
      Math.floor(bracket01(blue) * 127))

  # Generates a color value with the given hue, saturation, and
  # brightness values.
  #
  # @param {float} hue between 0 and 360
  # @param {float} saturation between 0 and 1
  # @param {float} brightness between 0 and 1
  # @return {Color}
  @hsb: (hue, saturation, brightness) ->
    hsvUnchecked(hue % 360.0, bracket01(saturation), bracket01(brightness))

#  @red: (c) -> redPart(c) / 127.0
#  @green: (c) -> greenPart(c) / 127.0
#  @blue: (c) -> bluePart(c) / 127.0

  # @property {Color}
  @BLACK: 0

  # @property {Color}
  @WHITE: 0x7f7f7f
