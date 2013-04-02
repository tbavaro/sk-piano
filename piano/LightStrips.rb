require "./Colors"

class LightStrip
  def initialize(num_pixels)
    @num_pixels = num_pixels
  end

  def num_pixels
    @num_pixels
  end

  def reset
    (0...num_pixels).each { |i| set_pixel(i, Colors.BLACK) }
  end

  def pixels
    result = Array.new(@num_pixels)
    (0...num_pixels).each { |i| result[i] = get_pixel(i) }
    result
  end

  def set_pixel(n, c)
    throw "abstract"
  end

  def get_pixel(n)
    throw "abstract"
  end
end
  
class FrameBufferLightStrip < LightStrip
  def initialize(num_pixels)
    super(num_pixels)
    @pixels = Array.new(num_pixels)
    reset
  end

  def reset
    @pixels.fill(Colors.BLACK)
  end

  def pixels
    @pixels
  end

  def get_pixel(n)
    (n < 0 or n >= @num_pixels) ? Colors.BLACK : @pixels[n]
  end

  def set_pixel(n, c)
    @pixels[n] = c if (n >= 0 and n < @num_pixels)
  end
end

class LogicalLightStrip < LightStrip
  def initialize(delegate, pixel_mapping)
    super(pixel_mapping.length)
    @delegate = delegate
    @pixel_mapping = pixel_mapping
  end

  def reset
    @pixel_mapping.each { |i| @delegate.set_pixel(i, Colors.BLACK) }
  end

  def get_pixel(n)
    (n < 0 or n >= @num_pixels) ? Colors.BLACK : @delegate.get_pixel(@pixel_mapping[n])
  end

  def set_pixel(n, c)
    @delegate.set_pixel(@pixel_mapping[n], c) if (n >= 0 and n < @num_pixels)
  end
end

