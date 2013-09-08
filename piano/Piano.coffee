Sleep = require("sleep")
FrameBufferLightStrip = require("./FrameBufferLightStrip")
TwinkleVisualizer = require("./TwinkleVisualizer")

TARGET_FPS = 30
NUM_PIXELS = 686

class Piano
  constructor: () ->
    @target_frame_duration = 1000 / TARGET_FPS
    @next_frame_time = Date.now() + @target_frame_duration

  pressedKeys: () ->
    throw "abstract"

  setLeds: (pixels) ->
    throw "abstract"

  throttle: () ->
    now = Date.now()
    delay_ms = @next_frame_time - now
    if delay_ms > 0
      Sleep.sleep(delay_ms / 1000.0)
      @next_frame_time += @target_frame_duration
    else
      @next_frame_time = now + @target_frame_duration

  runLoop: () ->
    counter = 0
    last_fps_time = 0
    render_time_since_last_check = 0
    set_led_time_since_last_check = 0
    frames_since_last_check = 0
    show_fps_every_n_frames = TARGET_FPS
    light_strip = new FrameBufferLightStrip(NUM_PIXELS)
    visualizer = new TwinkleVisualizer(light_strip)
    #    visualizer = TestVisualizer.new(light_strip)
    light_strip.reset()
    loop

      # scan to see which keys are pressed
      start_time = Date.now()

      visualizer.setPressedKeys(this.pressedKeys())

      render_end_time = Date.now()
      render_time_since_last_check += (render_end_time - start_time)

      # set the LEDs
      this.setLeds(light_strip.pixels())

      set_led_end_time = Date.now()
      set_led_time_since_last_check += (set_led_end_time - render_end_time)
      frames_since_last_check += 1

      # sleep to keep the FPS consistent
      this.throttle()

      if counter % show_fps_every_n_frames == 0
        now = Date.now()
        if last_fps_time != 0
          fps = show_fps_every_n_frames / ((now - last_fps_time) / 1000.0)
          console.log(fps, "fps /",
            render_time_since_last_check / frames_since_last_check,
            "per frame render /",
            set_led_time_since_last_check / frames_since_last_check,
            "per frame LEDs")
          render_time_since_last_check = 0
          set_led_time_since_last_check = 0
          frames_since_last_check = 0

        last_fps_time = now

      counter += 1

    return

module.exports = Piano