#include "BeagleBone.h"
#include "Util.h"

#include <stdio.h>
    
Pin::Pin(const char* value_file_name, const char* mode_file_name, 
         const char* export_key, const char* direction_file_name)
    : value_file_name(value_file_name),
      mode_file_name(mode_file_name),
      export_key(export_key),
      direction_file_name(direction_file_name) {
}

Pin::~Pin() {
  if (value_file != NULL) {
    fclose(value_file);
  }
}

const char* Pin::name() const {
  return value_file_name;
}

Pin Pin::P8_3(
    "/sys/class/gpio/gpio38/value",
    "/sys/kernel/debug/omap_mux/gpmc_ad6",
    "38",
    "/sys/class/gpio/gpio38/direction");

Pin Pin::P8_4(
    "/sys/class/gpio/gpio39/value",
    "/sys/kernel/debug/omap_mux/gpmc_ad7",
    "39",
    "/sys/class/gpio/gpio39/direction");

Pin Pin::USER_LED_3(
    "/sys/devices/platform/leds-gpio/leds/beaglebone::usr3/brightness",
    NULL, NULL, NULL);

static void writeValue(const char* filename, const char* value) {
  FILE* f = fopen(filename, "w");
  if (!f) {
    Util::fatal("can't open file: %s", filename);
  }
  fputs(value, f);
  fclose(f);
}

void Pin::setPinMode(PinMode pin_mode) {
  printf("Setting pin %s to mode %d\n", name(), pin_mode);
  
  if (mode_file_name == NULL) {
    if (pin_mode == OUTPUT) {
      //ok; LED pins are always output
      return;
    } else {
      Util::fatal("can't set pin %s to mode %d", name(), pin_mode);
    }
  }

  // set mux mode 7
  writeValue(mode_file_name, "7");

  // export the pin
  writeValue("/sys/class/gpio/export", export_key);

  // set it to output
  writeValue(direction_file_name, "out");
}

void Pin::digitalWrite(bool value) {
  if (value_file == NULL) {
    value_file = fopen(value_file_name, "w");
    if (value_file == NULL) {
      Util::fatal("unable to open pin file: %s", value_file_name);
    }
  }
  fputc(value ? '1' : '0', value_file);
  fflush(value_file);
}

