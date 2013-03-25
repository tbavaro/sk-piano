class Colors
  def self.BLACK
    0
  end

  def self.WHITE
    0x7f7f7f
  end

  def self._bracket(v, min = 0.0, max = 1.0)
    [[v, min].max, max].min
  end
  
  def self.rgb(r, g, b)
    (g << 16) | (r << 8) | (b)
  end

  # hue (0-360), saturation (0-1), value (0-1)
  def self.hsv(h, s, v)
    h = h % 360.0
    s = _bracket(s * 1.0)
    v = _bracket(v * 1.0)
    if (s == 0)
      r = g = b = v
    else
      h /= 60.0  # sector 0 to 5
      i = h.floor
      f = h - i # factorial part of h
      p = v * (1.0 - s)
      q = v * (1.0 - s * f)
      t = v * (1.0 - s * (1.0 - f))
      case i
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
      else
        r = v
        g = p
        b = q
      end
    end
    rgb((r * 127).floor, (g * 127).floor, (b * 127).floor)
  end

  def self.red(c)
    0x7f & (c >> 8)
  end

  def self.green(c)
    0x7f & (c >> 16)
  end

  def self.blue(c)
    0x7f & c
  end

  def self.add(a, b)
    rgb(
        _bracket(red(a) + red(b), 0, 127),
        _bracket(green(a) + green(b), 0, 127),
        _bracket(blue(a) + blue(b), 0, 127))
  end
end

