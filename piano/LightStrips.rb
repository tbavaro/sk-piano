require "./Colors"

class LightStrip
  def initialize(numPixels)
    @numPixels = numPixels
  end

  def numPixels
    @numPixels
  end

  def reset
    for i in 0...numPixels
      setPixel(i, Colors.BLACK)
    end
  end

  def pixels
    result = Array.new(@numPixels)
    for i in 0...numPixels
      result[i] = getPixel(i)
    end
    result
  end

  def setPixel(n, c)
    throw "abstract"
  end

  def getPixel(n)
    throw "abstract"
  end
end
  
class FrameBufferLightStrip < LightStrip
  def initialize(numPixels)
    super(numPixels)
    @pixels = Array.new(numPixels)
    reset
  end

  def reset
    @pixels.fill(Colors.BLACK)
  end

  def pixels
    @pixels
  end

  def getPixel(n)
    (n < 0 or n >= @numPixels) ? Colors.BLACK : @pixels[n]
  end

  def setPixel(n, c)
    @pixels[n] = c if (n >= 0 and n < @numPixels)
  end
end

class LogicalLightStrip < LightStrip
  def initialize(delegate, pixelMapping)
    super(pixelMapping.length)
    @delegate = delegate
    @pixelMapping = pixelMapping
  end

  def reset
    @pixelMapping.each { |i| @delegate.setPixel(i, Colors.BLACK) }
  end

  def getPixel(n)
    (n < 0 or n >= @numPixels) ? Colors.BLACK : @delegate.getPixel(@pixelMapping[n])
  end

  def setPixel(n, c)
    @delegate.setPixel(@pixelMapping[n], c) if (n >= 0 and n < @numPixels)
  end
end

