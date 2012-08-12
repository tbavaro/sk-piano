#include "BeagleBone.h"
#include "Util.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

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

SPI::SPI(uint32_t _max_speed_hz) : max_speed_hz(_max_speed_hz) {
  const char* spi_device = "/dev/spidev2.0";
  
  fd = open(spi_device, O_RDWR, 0);
  if (fd < 0) {
    Util::fatal("unable to open SPI device: %s", spi_device);
  }

  uint8_t tmp8;
  uint32_t tmp32;

  if (ioctl(fd, SPI_IOC_RD_MODE, &tmp8) == -1) {
    Util::fatal("unable to get read mode");
  } else if (tmp8 != 0) {
    Util::fatal("only mode 0 supported");
  }
  
  if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp8) == -1) {
    Util::fatal("unable to get bits per word");
  } else if (tmp8 != 8) {
    Util::fatal("only 8 bits per word supported");
  }

  ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &max_speed_hz);
  
  if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp32) == -1) {
    Util::fatal("unable to get max speed");
  }
  printf("max speed: %d\n", tmp32);

  printf("spi initialized!\n");
}

SPI::~SPI() {
  close(fd);
}

void SPI::send(void* buf, int num_bytes) {
  uint8_t rx_buf[num_bytes];
  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)buf;
  tr.rx_buf = (unsigned long)rx_buf;
  tr.len = num_bytes;
  tr.delay_usecs = 5;
  tr.speed_hz = max_speed_hz;
  tr.bits_per_word = 8;

  int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1) {
    Util::fatal("unable to send SPI message");
  }
}

