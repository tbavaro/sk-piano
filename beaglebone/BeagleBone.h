#ifndef __INCLUDED_BEAGLEBONE_H
#define __INCLUDED_BEAGLEBONE_H

#include <stdio.h>
#include <stdint.h>

#define ON true
#define OFF false

typedef enum {
  INPUT,
  OUTPUT
} PinMode;

class BeagleBone;

class Pin {
  friend class BeagleBone;

  public:
    static Pin P8_3;
    static Pin P8_4;
    static Pin USER_LED_3;

    const char* name() const;
    void setPinMode(PinMode pin_mode);
    void digitalWrite(bool value);

  private:
    Pin(const char* value_file_name, const char* mode_file_name, 
        const char* export_key, const char* direction_file_name);
    ~Pin();
    FILE* value_file;
    const char* value_file_name;

    // gpio pins only
    const char* mode_file_name;
    const char* export_key;
    const char* direction_file_name;
};

class SPI {
  public:
    SPI(uint32_t max_speed_hz);
    ~SPI();

    void send(void* buf, int num_bytes);

  private:
    int fd;
    uint32_t max_speed_hz;
};

#endif

