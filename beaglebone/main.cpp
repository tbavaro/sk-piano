#include "BeagleBone.h"
#include "Colors.h"
#include "CometVisualizer.h"
#include "DebugVisualizer.h"
#include "LogicalLightStrip.h"
#include "PhysicalLightStrip.h"
#include "SimulatorLightStrip.h"
#include "MasterVisualizer.h"
#include "SimpleParticleVisualizer.h"
#include "SimpleVisualizer.h"
#include "Piano.h"
#include "PhysicalPiano.h"
#include "SimulatorPiano.h"
#include "Util.h"

#include <stdio.h>
#include <unistd.h>

static int num_pixels = 1000;

static void showRainbow(LightStrip& strip) {
  int offset = 0;

  uint32_t last_time = Util::millis();

  int x = 0;

  int counter = 0;

  while(true) {
    for (int i = 0; i < num_pixels; ++i) {
      if ((counter % num_pixels) == i) {
        strip.setPixel(i, Colors::rainbow((i* 6  + offset) % 360));
      } else {
        strip.setPixel(i, Colors::BLACK);
      }
    }

    strip.show();

    Util::delay(30);

    if ((++counter % 30) == 0) {
      uint32_t millis = Util::millis();
      printf("frame %d fps\n", 30000 / (millis - last_time));
      last_time = millis;
    }

    ++offset;
    x = (x + 1) % 10;

  }
}

static void backAndForth(LightStrip& strip) {
  int pos = 0;
  int direction = 1;
  while(true) {
    strip.setPixel(pos, 0);
    pos += direction;
    if (pos < 0 || pos >= num_pixels) {
      direction *= -1;
      pos += 2 * direction;
    }
    strip.setPixel(pos, 0x7f7f7f);
    strip.show();
    
    Util::delay(30);
  }
}

static void christmas(LightStrip& strip) {
  for (int i = 0; i < num_pixels; ++i) {
    if ((i / 10) % 2 == 0) {
      strip.setPixel(i, Colors::rgb(127, 0, 0));
    } else {
      strip.setPixel(i, Colors::rgb(0, 127, 0));
    }
  }
  strip.show();
}

static void ranges(LightStrip& strip) {
  for (int i = 0; i < num_pixels; ++i) {
    Color c;
    if (i < 80) {
      // back half of top
      c = Colors::rgb(127, 0, 0);
    } else if (i < 92) {
      // front right half of top
      c = Colors::rgb(127, 0, 0);
    } else if (i < 136) {
      // front of top row
      c = Colors::rgb(0, 127, 0);
    } else if (i < 148) {
      // front half of top
      c = Colors::rgb(127, 0, 0);
    } else if (i < 163) {
      // front left 2nd row from top
      c = Colors::rgb(0, 0, 127);
    } else if (i < 204) {
      // directly above keys
      c = Colors::rgb(127, 127, 127);
    } else if (i < 219) {
      // front right 2nd row from top
      c = Colors::rgb(127, 127, 0);
    } else if (i < 298) {
      // back 2nd row from top
      c = Colors::rgb(127, 0, 127);
    } else if (i < 320) {
      // right 2nd row from bottom
      c = Colors::rgb(0, 127, 127);
    } else if (i < 364) {
      // front 2nd row from bottom
      c = Colors::rgb(127, 0, 0);
    } else if (i < 458) {
      // back 2nd row from bottom
      c = Colors::rgb(0, 127, 0);
    } else if (i < 478) {
      // front left bottom row
      c = Colors::rgb(0, 0, 127);
    } else if (i < 522) {
      // front bottom row
      c = Colors::rgb(127, 0, 127);
    } else if (i < 542) {
      // front right bottom row
      c = Colors::rgb(0, 127, 127);
    } else {
      c = Colors::rgb(0, 0, 0);
    }
    strip.setPixel(i, c);
  }
  strip.show();
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
    for(int i = 0; i < num_pixels; ++i) {
      strip.setPixel(i, color);
    }
    strip.show();

//    Util::delay(10);
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

static void piano(LightStrip& strip) {
  MasterVisualizer master_viz;

  // add visualizers
  master_viz.addVisualizer(new SimpleVisualizer(strip));
  master_viz.addVisualizer(new SimpleParticleVisualizer(strip, 300));
//  master_viz.addVisualizer(new CometVisualizer(strip, 300));
//  master_viz.addVisualizer(new DebugVisualizer());

#ifdef PIANO_SIMULATOR
  SimulatorPiano piano(&master_viz);
#else
  PhysicalPiano piano(&master_viz);
#endif
  while(true) {
    uint32_t frame_start = Util::millis();

    // read piano and fire key up/down and passFinished events
    piano.scan();

    // update LED strip
    strip.show();

    // throttle
    Util::delay_until(frame_start + 5);
  }
}

int main(int argc, char** argv) {
#ifdef PIANO_SIMULATOR
  SimulatorLightStrip strip(num_pixels);
#else
  PhysicalLightStrip strip(SPI(8e6), num_pixels);
#endif

  strip.reset();
  strip.show();
  strip.show();

//  blinkForever(Pin::P8_4);
//  showRainbow(strip);
//  backAndForth(strip);
//  glow(strip);

//  readTest(Pin::pin(8, 7), Pin::pin(8, 22));
//  piano(strip);
  ranges(strip);
//  getc(stdin);
//  christmas(strip);
}
