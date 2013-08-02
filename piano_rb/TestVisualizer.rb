require "./Colors"
require "./LightStrips"
require "./PianoLocations"
require "./Visualizers"

class TestVisualizerHelper < AmplitudeVisualizer
  def initialize(light_strip, hue, saturation, note_increase = 0.15, decrease_rate = 0.3)
    super(light_strip, note_increase, decrease_rate, 1.0)
    @light_strip = light_strip
    @hue = hue
    @saturation = saturation
  end

  def render_value(value)
    c = Colors.hsv(@hue, @saturation, value)
    (0...@light_strip.num_pixels).each { |i| @light_strip.set_pixel(i, c) }
  end
end

class TestVisualizer < CompositeVisualizer
  def initialize(light_strip)
    super [
      TestVisualizerHelper.new(light_strip, 0.0, 1.0),
      TestVisualizerHelper.new(
          LogicalLightStrip.new(light_strip,
            PianoLocations.top_front_row + PianoLocations.directly_above_keys),
          0.0, 0.0, 1.0, 5.0)
    ]
  end
end

