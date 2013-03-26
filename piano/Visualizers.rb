class Visualizer
  def reset
    setPressedKeys([])
  end

  def setPressedKeys(pressedKeys)
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

  def setPressedKeys(pressedKeys)
    newKeys = @keys2
    newKeys.fill(false)
    pressedKeys.each { |key| newKeys[key] = true }
    for key in 0...@@num_keys
      if newKeys[key] != @keys[key]
        newKeys[key] ? onKeyDown(key) : onKeyUp(key)
      end
    end
    @keys2 = @keys
    @keys = newKeys
    onPassFinished
  end

  def onKeyDown(key)
  end

  def onKeyUp(key)
  end

  def onPassFinished
  end
end

class TestKeyUpDownVisualizer < KeyUpDownVisualizer
  def onKeyDown(key)
    puts "key down: #{key}"
  end

  def onKeyUp(key)
    puts "key up: #{key}"
  end

  def onPassFinished
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

  def setPressedKeys(pressedKeys)
    @visualizers.each { |v| v.setPressedKeys(pressedKeys) }
  end
end

class AmplitudeVisualizer < KeyUpDownVisualizer
  def initialize(strip, noteIncrease, decreaseRate, maxValue)
    super()
    @strip = strip
    @noteIncrease = noteIncrease
    @decreaseRate = decreaseRate
    @maxValue = maxValue
    @prevFrameTime = 0
    @value = 0
  end

  def onKeyDown(key)
    @value = [@value + @noteIncrease, @maxValue].min
  end

  def onPassFinished
    now = Time.now.to_f
    if @prevFrameTime != 0
      secs = (now - @prevFrameTime)
      @value = [@value - (secs * @decreaseRate), 0].max
    end
    @prevFrameTime = now
    renderValue(@value)
  end

  def renderValue(value)
    throw "abstract"
  end

  def reset
    @prevFrameTime = 0
    @value = 0
    renderValue(@value)
  end
end

