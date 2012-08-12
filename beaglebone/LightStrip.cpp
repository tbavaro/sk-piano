#include "LightStrip.h"

#include <string.h>

//LightStrip::LightStrip(int num_leds, Pin& data_pin, Pin& clock_pin)
//  : num_leds(num_leds), data_pin(data_pin), clock_pin(clock_pin) {
LightStrip::LightStrip(SPI& spi, int num_leds) : spi(spi), num_leds(num_leds) {
  frame_buffer = new uint8_t[num_leds * 3 + 1];
  memset(frame_buffer, num_leds * 3 + 1, sizeof(uint8_t));
}

LightStrip::~LightStrip() {
  delete[] frame_buffer;
}

void LightStrip::setPixelColor(int n, Color c) {
  if (n < num_leds) {
    uint8_t* p = &frame_buffer[n * 3];
    *p++ = (c >> 16) | 0x80;
    *p++ = (c >> 8)  | 0x80;
    *p++ =  c        | 0x80;
  }
}

void LightStrip::show() {
  int num_bytes = num_leds * 3 + 1;
//  for (int i = 0; i < num_bytes; ++i) {
//    uint8_t byte = frame_buffer[i];
//    for (uint8_t bit = 0x80; bit; bit >>= 1) {
//      data_pin.digitalWrite(byte & bit);
//      clock_pin.digitalWrite(ON);
//      clock_pin.digitalWrite(OFF);
//    }
//  }
  spi.send(frame_buffer, num_bytes);
}

