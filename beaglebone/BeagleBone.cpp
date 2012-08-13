#include "BeagleBone.h"
#include "Util.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

Pin::Pin(const char* pin_name, int pin_number) {
  sprintf(value_file_name, "/sys/class/gpio/gpio%d/value", pin_number);
  sprintf(mode_file_name, "/sys/kernel/debug/omap_mux/%s", pin_name);
  sprintf(export_key, "%d", pin_number);
  sprintf(direction_file_name, "/sys/class/gpio/gpio%d/direction", pin_number);
}

Pin::~Pin() {
  if (value_file != NULL) {
    fclose(value_file);
  }
}

const char* Pin::name() const {
  return value_file_name;
}

static Pin* HEADER_8_PINS[] = {
  // 1:
  NULL,
  NULL,
  new Pin("gpmc_ad6", 38),
  new Pin("gpmc_ad7", 39),
  new Pin("gpmc_ad2", 34),

  // 6:
  NULL, //  new Pin("gpmc_ad3", 35), // something's wrong here, maybe reserved?
  new Pin("gpmc_advn_ale", 66),
  new Pin("gpmc_oen_ren", 67),
  new Pin("gpmc_ben0_cle", 69),
  new Pin("gpmc_wen", 68),

  // 11:
  new Pin("gpmc_ad13", 45),
  new Pin("gpmc_ad12", 44),
  new Pin("gpmc_ad9", 23),
  new Pin("gpmc_ad10", 26),
  new Pin("gpmc_ad15", 47),

  // 16:
  new Pin("gpmc_ad14", 46),
  new Pin("gpmc_ad11", 27),
  new Pin("gpmc_clk", 65),
  new Pin("gpmc_ad8", 22),
  new Pin("gpmc_csn2", 63),

  // 21:
  new Pin("gpmc_csn1", 62),
  new Pin("gpmc_ad5", 37),
  new Pin("gpmc_ad4", 36),
  new Pin("gpmc_ad1", 33),
  new Pin("gpmc_ad0", 32),

  // 26:
  new Pin("gpmc_csn0", 61)
};
static int NUM_HEADER_8_PINS = sizeof(HEADER_8_PINS) / sizeof(HEADER_8_PINS[0]);
    
Pin& Pin::pin(int header, int pin_num) {
  Pin** pins;
  int num_pins;
  if (header == 8) {
    pins = HEADER_8_PINS;
    num_pins = NUM_HEADER_8_PINS;
  } else {
    Util::fatal("illegal pin: %d:%d", header, pin_num);
  }

  if (pin_num < 1 || pin_num > num_pins) {
    Util::fatal("illegal pin: %d:%d", header, pin_num);
  }

  Pin* pin = pins[pin_num - 1];
  if (pin == NULL) {
    Util::fatal("illegal pin: %d:%d", header, pin_num);
  }

  return *pin;
}

static void writeValue(const char* filename, const char* value) {
  FILE* f = fopen(filename, "w");
  if (!f) {
    Util::fatal("can't open file: %s", filename);
  }
  fputs(value, f);
  fclose(f);
}

static bool readValue(const char* filename) {
  FILE* f = fopen(filename, "r");
  if (!f) {
    Util::fatal("can't open file: %s", filename);
  }
  char c = fgetc(f);
//  fprintf(stderr, "%c", c);
  fclose(f);
  return (c == '0');
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

  // export the pin
  writeValue("/sys/class/gpio/export", export_key);

  // set it to output
  writeValue(direction_file_name, pin_mode == INPUT ? "in" : "out");

  // set mux mode 7
  writeValue(mode_file_name, pin_mode == INPUT ? "27" : "7");
}

inline FILE* Pin::getOrOpenValueFile() {
  if (value_file == NULL) {
    value_file = fopen(value_file_name, "w"); //xcxc
    if (value_file == NULL) {
      Util::fatal("unable to open pin file: %s", value_file_name);
    }
  }
  return value_file;
}

bool Pin::digitalRead() {
  return readValue(value_file_name);
}

void Pin::digitalWrite(bool value) {
  FILE* f = getOrOpenValueFile();
  fputc(value ? '1' : '0', f);
  fflush(f);
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

// SPI pins: 31 CLK, 30 DATA

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

