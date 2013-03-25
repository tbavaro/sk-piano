class Colors
  def self.BLACK
    0x808080
  end

  def self.WHITE
    0xffffff
  end

  def self.rgb(r, g, b)
    0x808080 | (g << 16) | (r << 8) | (b)
  end

  # hue (0-360), saturation (0-1), value (0-1)
  def self.hsv(h, s, v)
    h = h % 360.0
    s = [[0.0, s * 1.0].max, 1.0].min
    v = [[0.0, v * 1.0].max, 1.0].min
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
end

