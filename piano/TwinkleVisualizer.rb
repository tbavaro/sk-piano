require "./Colors"
require "./LightStrips"
require "./PianoLocations"
require "./Visualizers"

class TwinkleVisualizerHelper < AmplitudeVisualizer
  alias :super_reset :reset
  alias :super_setPressedKeys :setPressedKeys

  def initialize(lightStrip, highlightKeys)
    super(lightStrip, 0.15, 0.3, 1.2)
    @lightStrip = lightStrip
    @highlightKeys = highlightKeys
    reset
  end

  def reset
    @pixelValues = Array.new(@lightStrip.numPixels, 0.0)
    @pixelSaturations = Array.new(@lightStrip.numPixels, 0.0)
    @pressedKeys = []
    @sparkleAccum = 0
    super_reset()
  end

  def pixelForKey(key)
    (key / 88.0 * @lightStrip.numPixels).floor
  end

  def setPressedKeys(pressedKeys)
    @pressedKeys = pressedKeys
    super_setPressedKeys(pressedKeys)
  end

  def renderValue(value)
    value = [1.0, value].min

    # decay pixels
    @pixelValues.map! { |v| v * 0.8 }

    # reset some pixels to 1
    @sparkleAccum = @lightStrip.numPixels * (0.001 + ((value ** 0.8) * 0.05))
    for i in 0...@sparkleAccum.floor
      pixel = rand(@lightStrip.numPixels)
      @pixelValues[pixel] = 1.0
    end
    @sparkleAccum = @sparkleAccum % 1.0

    if (@highlightKeys)
      # decay saturations
      @pixelSaturations.map! { |v| v * 0.9 }

      # set saturations for pressed keys
      radius = (@lightStrip.numPixels / 88.0 * 2.0).floor
      @pressedKeys.each do |key|
        pixel = pixelForKey(key)
        leftPixel = [0, pixel - radius].max
        rightPixel = [@lightStrip.numPixels - 1, pixel + radius].min
        for i in leftPixel..rightPixel
          pixelSaturations[i] = 1.0
        end
      end
    end

    # draw
    overallSaturation = [1.0, value ** 2.0].min
    hue = (Time.now.to_f * 1000.0 / 30.0)  % 360.0
    for i in 0...(@lightStrip.numPixels)
      saturation = 0.0
      brightness = 0.0
      if (@highlightKeys)
        saturation = [0.0, overallSaturation - @pixelSaturations[i]].max
        brightness = [1.0, @pixelValues[i] * 0.3 + @pixelSaturations[i] * 0.7].min
      else
        saturation = overallSaturation
        brightness = [1.0, @pixelValues[i]].min
      end
      @lightStrip.setPixel(i, Colors.hsv(hue, saturation, brightness))
    end
  end
end

class TwinkleVisualizer < CompositeVisualizer
  def initialize(lightStrip)
    super [
      TwinkleVisualizerHelper.new(lightStrip, false),
      TwinkleVisualizerHelper.new(
          LogicalLightStrip.new(lightStrip, PianoLocations.topFrontRow),
          true),
      TwinkleVisualizerHelper.new(
          LogicalLightStrip.new(lightStrip, PianoLocations.directlyAboveKeys),
          true)
    ]
  end
end
