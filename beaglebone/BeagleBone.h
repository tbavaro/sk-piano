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

class Pin {
  public:
    static Pin& pin(int header, int pin);
    static Pin USER_LED_3;

    const char* name() const;
    void setPinMode(PinMode pin_mode);
    bool digitalRead();
    void digitalWrite(bool value);

    Pin(const char* pin_name, int pin_number);
    ~Pin();

  private:
    FILE* getOrOpenValueFile();
    FILE* value_file;

    char value_file_name[64];
    char mode_file_name[64];
    char export_key[64];
    char direction_file_name[64];
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

