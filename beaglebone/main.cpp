#include "AmplitudeMeterVisualizer.h"
#include "AmplitudeGlowVisualizer.h"
#include "BeagleBone.h"
#include "Colors.h"
#include "CometVisualizer.h"
#include "CompositeVisualizer.h"
#include "DebugVisualizer.h"
#include "DaveeyVisualizer.h"
#include "LogicalLightStrip.h"
#include "PhysicalLightStrip.h"
#include "MasterVisualizer.h"
#include "RainbowVisualizer.h"
#include "SimpleParticleVisualizer.h"
#include "SimpleVisualizer.h"
#include "StackedVisualizer.h"
#include "PhysicalPiano.h"
#include "PianoLocations.h"
#include "Util.h"

#include "simulator/SimulatorLightStrip.h"
#include "simulator/SimulatorPiano.h"

#include <stdio.h>
#include <unistd.h>

static const int NUM_PIXELS = 686;
static const int MAX_FPS = 30;

static void throttleFrameRate() {
  static const uint32_t millis_per_frame = 1000 / MAX_FPS;
  static uint32_t prev_frame_time = 0;
  Util::delay_until(prev_frame_time + millis_per_frame);
  prev_frame_time = Util::millis();
}

static void backAndForth(LightStrip& strip) {
  int pos = 0;
  int direction = 1;
  while(true) {
    strip.setPixel(pos, 0);
    pos += direction;
    if (pos < 0 || pos >= NUM_PIXELS) {
      direction *= -1;
      pos += 2 * direction;
    }
    strip.setPixel(pos, 0x7f7f7f);
    strip.show();

    throttleFrameRate();
  }
}

static void christmas(LightStrip& strip) {
  while(1) {
    for (int i = 0; i < NUM_PIXELS; ++i) {
      if ((i / 10) % 2 == 0) {
        strip.setPixel(i, Colors::rgb(127, 0, 0));
      } else {
        strip.setPixel(i, Colors::rgb(0, 127, 0));
      }
    }
    strip.show();

    throttleFrameRate();
  }
}

static void ranges(LightStrip& strip) {
  while(true) {
    for (int i = 0; i < NUM_PIXELS; ++i) {
      Color c;
      if (i < 80) {
        // back half of top
        c = Colors::rgb(127, 0, 0); //red
      } else if (i < 92) {
        // front right half of top
        c = Colors::rgb(127, 0, 0); //red
      } else if (i < 136) {
        // front of top row
        c = Colors::rgb(0, 127, 0); //green
      } else if (i < 148) {
        // front half of top
        c = Colors::rgb(127, 0, 0); //red
      } else if (i < 163) {
        // front left 2nd row from top
        c = Colors::rgb(0, 0, 127); //blue
      } else if (i < 204) {
        // directly above keys
        c = Colors::rgb(127, 127, 127); //white
      } else if (i < 219) {
        // front right 2nd row from top
        c = Colors::rgb(127, 127, 0); //yellow
      } else if (i < 298) {
        // back 2nd row from top
        c = Colors::rgb(127, 0, 127); //purple
      } else if (i < 320) {
        // right 2nd row from bottom
        c = Colors::rgb(0, 127, 127); //cyan
      } else if (i < 364) {
        // front 2nd row from bottom
        c = Colors::rgb(127, 0, 0); //red
      } else if (i < 458) {
        // back 2nd row from bottom
        c = Colors::rgb(0, 127, 0); //green
      } else if (i < 478) {
        // front left bottom row
        c = Colors::rgb(0, 0, 127); //blue
      } else if (i < 522) {
        // front bottom row
        c = Colors::rgb(127, 0, 127); //purple
      } else if (i < 542) {
        // front right bottom row
        c = Colors::rgb(0, 127, 127);
      } else if (i < 560) {
        // up the back of the right leg
        c = Colors::rgb(127, 127, 127);
      } else if (i < 578) {
        // down the front of the right leg
        c = Colors::rgb(127, 0, 0);
      } else {
        c = Colors::rgb(0, 0, 0);
      }
      strip.setPixel(i, c);
    }
    strip.show();

    throttleFrameRate();
  }
}

static void glow(LightStrip& strip) {
  int brightness = 0;
  int direction = 1;
  while(true) {
    brightness += direction;
    if (brightness < 1 || brightness > 127) {
      direction *= -1;
      brightness += 2 * direction;
    }

    Color color = Colors::hsv(0, 0.0, brightness / 127.0);
    for(int i = 0; i < NUM_PIXELS; ++i) {
      strip.setPixel(i, color);
    }
    strip.show();

    throttleFrameRate();
  }
}

static void blinkForever(Pin& pin) {
  pin.setPinMode(OUTPUT);
  while(true) {
    printf("pin ON\n");
    pin.digitalWrite(ON);
    sleep(1);
    printf("pin OFF\n");
    pin.digitalWrite(OFF);
    sleep(1);
  }
}

static void readTest(Pin& out_pin, Pin& in_pin) {
  out_pin.setPinMode(OUTPUT);
  out_pin.digitalWrite(ON);
  in_pin.setPinMode(INPUT);
  bool prev_value = false;
  int counter = 0;
  bool out_pin_value = true;
  while(true) {
    bool value = in_pin.digitalRead();
    if (value != prev_value) {
      Util::log("new value: %d", value);
      prev_value = value;
    }
    if(++counter % 1000 == 0) {
      fprintf(stderr, ".");
//      out_pin_value ^= true;
//      out_pin.digitalWrite(out_pin_value);
    }
  }
}

static void addLegAmplitudeMeters(
    CompositeVisualizer* vis, LightStrip& strip, float note_increase, 
    float decrease_rate, float max_value) {
  LogicalLightStrip* up_right_leg_front = 
      PianoLocations::upRightLegFront(strip);
  LogicalLightStrip* up_right_leg_rear = 
      PianoLocations::upRightLegRear(strip);
  LogicalLightStrip* up_back_leg_right_front = 
      PianoLocations::upBackLegFrontRight(strip);
  LogicalLightStrip* up_back_leg_right_rear = 
      PianoLocations::upBackLegRearRight(strip);
  LogicalLightStrip* up_back_leg_left_front = 
      PianoLocations::upBackLegFrontLeft(strip);
  LogicalLightStrip* up_back_leg_left_rear = 
      PianoLocations::upBackLegRearLeft(strip);
  LogicalLightStrip* up_left_leg_front = 
      PianoLocations::upLeftLegFront(strip);
  LogicalLightStrip* up_left_leg_rear = 
      PianoLocations::upLeftLegRear(strip);
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_right_leg_front, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_right_leg_rear, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_back_leg_right_front, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_back_leg_right_rear, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_back_leg_left_front, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_back_leg_left_rear, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_left_leg_front, note_increase, decrease_rate, max_value));
  vis->addVisualizer(
      new AmplitudeMeterVisualizer(
        *up_left_leg_rear, note_increase, decrease_rate, max_value));
}

static Visualizer* makeSceneOne(LightStrip& strip) {
  LogicalLightStrip* around_top = PianoLocations::aroundTopExcludingFrontRow(strip);
  LogicalLightStrip* around_second_row = PianoLocations::aroundSecondRowExcludingFrontRow(strip);
  LogicalLightStrip* top_front_row = PianoLocations::topFrontRow(strip);
  LogicalLightStrip* above_keyboard = PianoLocations::directlyAboveKeys(strip);
  
  CompositeVisualizer* vis = new CompositeVisualizer();
  vis->addVisualizer(new RainbowVisualizer(strip, 80, -100));
  vis->addVisualizer(new AmplitudeGlowVisualizer(*around_top, 0.1, 0.5, 3.0));
  vis->addVisualizer(new AmplitudeGlowVisualizer(*around_second_row, 0.1, 0.5, 3.0));
  vis->addVisualizer(new DaveeyVisualizer(*top_front_row));
  vis->addVisualizer(new DaveeyVisualizer(*above_keyboard));
  addLegAmplitudeMeters(vis, strip, 0.25, 1.0, 1.5);
  return vis;
}

static Visualizer* makeSceneTwo(LightStrip& strip) {
  CompositeVisualizer* vis = new CompositeVisualizer();
  
  LogicalLightStrip* entire_top = PianoLocations::aroundTopExcludingFrontRow(strip);
  vis->addVisualizer(new StackedVisualizer(*entire_top));
  
  LogicalLightStrip* entire_second_row = PianoLocations::aroundSecondRowExcludingFrontRow(strip);
  vis->addVisualizer(new StackedVisualizer(*entire_second_row));
  
  LogicalLightStrip* around_third_row = PianoLocations::aroundThirdRowExcludingFrontRow(strip);
  vis->addVisualizer(new StackedVisualizer(*around_third_row));
  
  LogicalLightStrip* around_bottom_row = PianoLocations::aroundBottomRowWithGapToMatchThirdRow(strip);
  vis->addVisualizer(new StackedVisualizer(*around_bottom_row));

  LogicalLightStrip* top_front_row = PianoLocations::topFrontRow(strip);
  vis->addVisualizer(new DaveeyVisualizer(*top_front_row));
  
  LogicalLightStrip* above_keyboard = PianoLocations::directlyAboveKeys(strip);
  vis->addVisualizer(new DaveeyVisualizer(*above_keyboard));

  LogicalLightStrip* third_front_row = PianoLocations::thirdFrontRow(strip);
  vis->addVisualizer(new DaveeyVisualizer(*third_front_row));
  
  LogicalLightStrip* bottom_front_row = PianoLocations::bottomFrontRow(strip);
  vis->addVisualizer(new DaveeyVisualizer(*bottom_front_row));
  
  return vis;
}

static void piano(LightStrip& strip) {
  MasterVisualizer master_viz(strip);

  // add visualizers
  LogicalLightStrip* above_keyboard = LogicalLightStrip::fromRange(strip, 163, 203);

  master_viz.addVisualizer(makeSceneOne(strip));
  master_viz.addVisualizer(makeSceneTwo(strip));
  master_viz.addVisualizer(new SimpleParticleVisualizer(strip, 1000));
//  master_viz.addVisualizer(new SimpleVisualizer(*above_keyboard));
//  master_viz.addVisualizer(new CometVisualizer(strip, 300));
//  master_viz.addVisualizer(new DebugVisualizer());

#ifdef PIANO_SIMULATOR
  SimulatorPiano piano(&master_viz);
#else
  PhysicalPiano piano(&master_viz);
#endif

  static const int SHOW_FPS_EVERY_N_FRAMES = MAX_FPS;
  int counter = 0;
  uint32_t last_fps_time = 0;
  while(true) {
    piano.scan();
    strip.show();

    throttleFrameRate();

    if ((counter++ % SHOW_FPS_EVERY_N_FRAMES) == 0) {
      uint32_t now = Util::millis();
      if (last_fps_time != 0) {
        uint32_t duration = now - last_fps_time;
        printf("fps: %d\n", (SHOW_FPS_EVERY_N_FRAMES * 1000) / duration);
      }
      last_fps_time = now;
    }
  }
}

int main(int argc, char** argv) {
#ifdef PIANO_SIMULATOR
  SimulatorLightStrip strip(NUM_PIXELS);
#else
  SPI spi(4e6);
  PhysicalLightStrip strip(spi, NUM_PIXELS);
#endif

  strip.reset();
  strip.show();
  strip.show();

//  blinkForever(Pin::P8_4);
//  backAndForth(strip);
//  glow(strip);

//  readTest(Pin::pin(8, 7), Pin::pin(8, 22));
  piano(strip);
//  ranges(strip);
// getc(stdin);
//  christmas(strip);
}
