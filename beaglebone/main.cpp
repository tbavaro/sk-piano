#include "BeagleBone.h"
#include "Colors.h"
#include "CometVisualizer.h"
#include "LightStrip.h"
#include "MasterVisualizer.h"
#include "SimpleParticleVisualizer.h"
#include "SimpleVisualizer.h"
#include "Piano.h"
#include "Util.h"

#include <stdio.h>
#include <unistd.h>

static int num_pins = 162;
static SPI spi(4e6);
static LightStrip strip(spi, num_pins);

static void showRainbow() {
  int offset = 0;

  uint32_t last_time = Util::millis();

  int x = 0;

  int counter = 0;

  while(true) {
    for (int i = 0; i < num_pins; ++i) {
      strip.setPixelColor(i, Colors::rainbow((i + offset) % 360));
    }

    strip.show();

//    Util::delay(10);

    if ((++counter % 30) == 0) {
      uint32_t millis = Util::millis();
      printf("frame %d fps\n", 30000 / (millis - last_time));
      last_time = millis;
    }

    ++offset;
    x = (x + 1) % 10;

  }
}

static void backAndForth() {
  int pos = 0;
  int direction = 1;
  while(true) {
    strip.setPixelColor(pos, 0);
    pos += direction;
    if (pos < 0 || pos >= num_pins) {
      direction *= -1;
      pos += 2 * direction;
    }
    strip.setPixelColor(pos, 0x7f7f7f);
    strip.show();
  }
}

static void glow() {
  int brightness = 0;
  int direction = 1;
  while(true) {
    brightness += direction;
    if (brightness < 1 || brightness > 127) {
      direction *= -1;
      brightness += 2 * direction;
    }

    Color color = Colors::hsv(0, 0.0, brightness / 127.0);
    for(int i = 0; i < num_pins; ++i) {
      strip.setPixelColor(i, color);
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
      out_pin_value ^= true;
      out_pin.digitalWrite(out_pin_value);
    }
  }
}

static Visualizer* makeSimpleParticleVisualizer() {
  return new SimpleParticleVisualizer(strip, 300);
}

static Visualizer* makeCometVisualizer() {
  return new CometVisualizer(strip, 300);
}

static Visualizer* makeSimpleVisualizer() {
  return new SimpleVisualizer(strip);
}

static void piano() {
  MasterVisualizer master_viz(strip);

  // add visualizers
  master_viz.addVisualizer(makeSimpleParticleVisualizer);
  master_viz.addVisualizer(makeCometVisualizer);
  master_viz.addVisualizer(makeSimpleVisualizer);

  Piano piano(&master_viz);
  while(true) {
    piano.checkOne();
  }
}

int main(int argc, char** argv) {
  strip.reset();
  strip.show();
  strip.show();

//  blinkForever(Pin::P8_4);
//  showRainbow();
//  backAndForth();
//  glow();

//  readTest(Pin::pin(8, 4), Pin::pin(8, 26));
  piano();
}
