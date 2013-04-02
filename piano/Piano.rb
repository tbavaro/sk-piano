require "./LightStrips"
require "./TestVisualizer"
require "./TwinkleVisualizer"

TARGET_FPS = 30
NUM_PIXELS = 686

class Piano
  def initialize
    @target_frame_duration = 1.0 / TARGET_FPS
    @next_frame_time = Time.now.to_f + @target_frame_duration
  end

  def pressed_keys
    throw "abstract"
  end

  def set_leds(pixels)
    throw "abstract"
  end

  def throttle
    now = Time.now.to_f
    if now < @next_frame_time
      sleep (@next_frame_time - now)
      @next_frame_time += @target_frame_duration
    else
      @next_frame_time = now + @target_frame_duration
    end
  end

  def loop
    counter = 0
    last_fps_time = 0
    render_time_since_last_check = 0.0
    set_led_time_since_last_check = 0.0
    frames_since_last_check = 0.0
    show_fps_every_n_frames = TARGET_FPS
    light_strip = FrameBufferLightStrip.new(NUM_PIXELS)
    visualizer = TwinkleVisualizer.new(light_strip)
#   visualizer = TestVisualizer.new(light_strip)
    light_strip.reset
    while true
      # scan to see which keys are pressed
      start_time = Time.now.to_f

      #visualizer.set_pressed_keys(pressed_keys)

      render_end_time = Time.now.to_f
      render_time_since_last_check += (render_end_time - start_time)

      # set the LEDs
      set_leds(light_strip.pixels)

      set_led_end_time = Time.now.to_f
      set_led_time_since_last_check += (set_led_end_time - render_end_time)
      frames_since_last_check += 1

      # sleep to keep the FPS consistent
      throttle
      
      if counter % show_fps_every_n_frames == 0
        now = Time.now.to_f
        if last_fps_time != 0
          fps = show_fps_every_n_frames / (now - last_fps_time)
          puts "%5.2f fps / %6.2fms per frame render / %6.2fms per frame LEDs" % [
              fps,
              (render_time_since_last_check / frames_since_last_check) * 1000.0,
              (set_led_time_since_last_check / frames_since_last_check) * 1000.0
          ]
          render_time_since_last_check = 0.0
          set_led_time_since_last_check = 0.0
          frames_since_last_check = 0
        end
        last_fps_time = now
      end
      counter += 1
    end
  end
end

