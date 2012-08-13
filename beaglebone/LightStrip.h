#ifndef __INCLUDED_LIGHT_STRIP_H
#define __INCLUDED_LIGHT_STRIP_H

#include "BeagleBone.h"
#include "Colors.h"

class LightStrip {
  public:
    LightStrip(SPI& spi, int num_leds);
//    LightStrip(int num_leds, Pin& data_pin, Pin& clock_pin);
    ~LightStrip();

    int numPixels();

    void reset();
    void addPixelColor(int n, Color color);
    void setPixelColor(int n, Color color);
    void show();

  private:
    SPI& spi;
    int num_leds;
//    Pin& data_pin;
//    Pin& clock_pin;
    uint8_t* frame_buffer;
};

#endif

