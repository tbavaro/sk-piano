class Colors
  def self.BLACK
    0
  end

  def self.WHITE
    0x7f7f7f
  end

  def self._bracket(v, min = 0.0, max = 1.0)
    if v < min
      min
    elsif v > max
      max
    else
      v
    end
  end
  
  def self.rgb(r, g, b)
    _rgb_unsafe(
        _bracket(r, 0, 127).floor,
        _bracket(g, 0, 127).floor,
        _bracket(b, 0, 127).floor)
  end

  def self._rgb_unsafe(r, g, b)
    # (g << 16) | (r << 8) | (b)
    [0, g, r, b].pack("cccc").unpack("N")[0]
  end

  # hue (0-360), saturation (0-1), value (0-1)
  def self.hsv(h, s, v)
    _hsv_unsafe(h % 360.0, _bracket(s * 1.0), _bracket(v * 1.0))
  end

  def self._hsv_unsafe(h, s, v)
    if s == 0
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
        r, g, b = v, t, p
      when 1
        r, g, b = q, v, p
      when 2
        r, g, b = p, v, t
      when 3
        r, g, b = p, q, v
      when 4
        r, g, b = t, p, v
      else
        r, g, b = v, p, q
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
    rgb(red(a) + red(b), green(a) + green(b), blue(a) + blue(b))
  end
end

