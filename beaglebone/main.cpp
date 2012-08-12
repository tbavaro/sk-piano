#include "BeagleBone.h"
#include "LightStrip.h"
#include "Util.h"

#include <stdio.h>
#include <unistd.h>

static void showRainbow() {
  int num_pins = 160;
  LightStrip strip(num_pins, Pin::USER_LED_3, Pin::USER_LED_3);

  for (int i = 0; i < num_pins; ++i) {
    strip.setPixelColor(i, Colors::rainbow((i * 12) % 360));
  }

  strip.show();
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
//  showRainbow();
  blinkForever(Pin::P8_4);
}
