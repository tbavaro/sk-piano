require "./Colors"
require "./LightStrips"
require "./PianoLocations"
require "./Visualizers"

class TwinkleVisualizerHelper < AmplitudeVisualizer
  alias :super_reset :reset
  alias :super_set_pressed_keys :set_pressed_keys

  def initialize(light_strip, highlight_keys)
    super(light_strip, 0.15, 0.3, 1.2)
    @light_strip = light_strip
    @highlight_keys = highlight_keys
    reset
  end

  def reset
    @pixel_values = Array.new(@light_strip.num_pixels, 0.0)
    @pixel_saturations = Array.new(@light_strip.num_pixels, 0.0)
    @pressed_keys = []
    @sparkle_accum = 0
    super_reset()
  end

  def pixel_for_key(key)
    (key / 88.0 * @light_strip.num_pixels).floor
  end

  def set_pressed_keys(pressed_keys)
    @pressed_keys = pressed_keys
    super_set_pressed_keys(pressed_keys)
  end

  def render_value(value)
    value = [1.0, value].min

    # decay pixels
    @pixel_values.map! { |v| v * 0.8 }

    # reset some pixels to 1
    @sparkle_accum += @light_strip.num_pixels * (0.001 + ((value ** 0.8) * 0.05))
    while @sparkle_accum >= 1.0
      pixel = rand(@light_strip.num_pixels)
      @pixel_values[pixel] = 1.0
      @sparkle_accum -= 1.0
    end

    if @highlight_keys
      # decay saturations
      @pixel_saturations.map! { |v| v * 0.9 }

      # set saturations for pressed keys
      radius = (@light_strip.num_pixels / 88.0 * 2.0).floor
      @pressed_keys.each do |key|
        pixel = pixel_for_key(key)
        left_pixel = [0, pixel - radius].max
        right_pixel = [@light_strip.num_pixels - 1, pixel + radius].min
        (left_pixel..right_pixel).each { |i| @pixel_saturations[i] = 1.0 }
      end
    end

    # draw
    overall_saturation = [1.0, value ** 2.0].min
    hue = (Time.now.to_f * 1000.0 / 30.0) % 360.0
    (0...@light_strip.num_pixels).each do |i|
      if @highlight_keys
        saturation = [0.0, overall_saturation - @pixel_saturations[i]].max
        brightness = [1.0, @pixel_values[i] * 0.3 + @pixel_saturations[i] * 0.7].min
      else
        saturation = overall_saturation
        brightness = [1.0, @pixel_values[i]].min
      end
#      puts "#{i} #{hue} #{saturation} #{brightness}"
      @light_strip.set_pixel(i, Colors.hsv(hue, saturation, brightness))
    end
  end
end

class TwinkleVisualizer < CompositeVisualizer
  def initialize(light_strip)
    super [
      TwinkleVisualizerHelper.new(light_strip, false),
      TwinkleVisualizerHelper.new(
          LogicalLightStrip.new(
              light_strip,
              PianoLocations.top_front_row), true),
      TwinkleVisualizerHelper.new(
          LogicalLightStrip.new(
              light_strip,
              PianoLocations.directly_above_keys), true)
    ]
  end
end
