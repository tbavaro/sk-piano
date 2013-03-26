require "./Colors"
require "./LightStrips"
require "./PianoLocations"
require "./Visualizers"

class TestVisualizerHelper < AmplitudeVisualizer
  def initialize(lightStrip, hue, saturation, noteIncrease = 0.15, decreaseRate = 0.3)
    super(lightStrip, noteIncrease, decreaseRate, 1.0)
    @lightStrip = lightStrip
    @hue = hue
    @saturation = saturation
  end

  def renderValue(value)
    c = Colors.hsv(@hue, @saturation, value)
    for i in 0...@lightStrip.numPixels
      @lightStrip.setPixel(i, c)
    end
  end
end

class TestVisualizer < CompositeVisualizer
  def initialize(lightStrip)
    super [
      TestVisualizerHelper.new(lightStrip, 0.0, 1.0),
      TestVisualizerHelper.new(
          LogicalLightStrip.new(lightStrip, 
            PianoLocations.topFrontRow + PianoLocations.directlyAboveKeys),
          0.0, 0.0, 1.0, 5.0)
    ]
  end
end

