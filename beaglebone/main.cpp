#include "BeagleBone.h"
#include "LightStrip.h"
#include "Util.h"

#include <stdio.h>
#include <unistd.h>

static void showRainbow(SPI& spi) {
  int num_pins = 1000;
  LightStrip strip(spi, num_pins); //, Pin::P8_3, Pin::P8_4);

  int offset = 0;

  uint32_t last_time = Util::millis();

  int x = 0;

  while(true) {
    for (int i = 0; i < num_pins; ++i) {
      if (i % 10 == x) {
        strip.setPixelColor(i, Colors::rainbow((i * 6 + offset / 100) % 360));
      } else {
        strip.setPixelColor(i, 0);
      }
    }

    strip.show();

    uint32_t millis = Util::millis();
    printf("frame %d\n", millis - last_time);
    last_time = millis;

    ++offset;
    x = (x + 1) % 10;

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

int main(int argc, char** argv) {
//  blinkForever(Pin::P8_4);
  SPI spi(8e6);
  showRainbow(spi);
}
