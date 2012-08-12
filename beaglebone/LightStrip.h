#ifndef __INCLUDED_LIGHT_STRIP_H
#define __INCLUDED_LIGHT_STRIP_H

#include "BeagleBone.h"
#include "Colors.h"

class LightStrip {
  public:
    LightStrip(int num_leds, Pin& data_pin, Pin& clock_pin);
    ~LightStrip();

    void setPixelColor(int n, Color color);
    void show();

  private:
    int num_leds;
    Pin& data_pin;
    Pin& clock_pin;
    uint8_t* frame_buffer;
};

#endif

