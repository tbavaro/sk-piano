class Visualizer
  def reset
    set_pressed_keys([])
  end

  def set_pressed_keys(pressed_keys)
    throw "abstract"
  end
end

class KeyUpDownVisualizer < Visualizer
  @@num_keys = 88

  def initialize
    # make 2 arrays that we switch between, so we don't have to keep 
    # making new arrays
    @keys = Array.new(@@num_keys, false)
    @keys2 = Array.new(@@num_keys, false)
  end

  def set_pressed_keys(pressed_keys)
    new_keys = @keys2
    new_keys.fill(false)
    pressed_keys.each { |key| new_keys[key] = true }
    for key in 0...@@num_keys
      if new_keys[key] != @keys[key]
        new_keys[key] ? on_key_down(key) : on_key_up(key)
      end
    end
    @keys2 = @keys
    @keys = new_keys
    on_pass_finished
  end

  def on_key_down(key)
  end

  def on_key_up(key)
  end

  def on_pass_finished
  end
end

class TestKeyUpDownVisualizer < KeyUpDownVisualizer
  def on_key_down(key)
    puts "key down: #{key}"
  end

  def on_key_up(key)
    puts "key up: #{key}"
  end

  def on_pass_finished
    puts "---"
  end
end

class CompositeVisualizer < Visualizer
  def initialize(visualizers)
    @visualizers = visualizers
  end

  def reset
    @visualizers.each { |v| v.reset }
  end

  def set_pressed_keys(pressed_keys)
    @visualizers.each { |v| v.set_pressed_keys(pressed_keys) }
  end
end

class AmplitudeVisualizer < KeyUpDownVisualizer
  def initialize(strip, note_increase, decrease_rate, max_value)
    super()
    @strip = strip
    @note_increase = note_increase
    @decrease_rate = decrease_rate
    @max_value = max_value
    @prev_frame_time = 0
    @value = 0
  end

  def on_key_down(key)
    @value = [@value + @note_increase, @max_value].min
  end

  def on_pass_finished
    now = Time.now.to_f
    if @prev_frame_time != 0
      secs = (now - @prev_frame_time)
      @value = [@value - (secs * @decrease_rate), 0].max
    end
    @prev_frame_time = now
    render_value(@value)
  end

  def render_value(value)
    throw "abstract"
  end

  def reset
    @prev_frame_time = 0
    @value = 0
    render_value(@value)
  end
end

