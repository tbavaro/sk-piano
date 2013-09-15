#include "MmapGpio.h"

#include "Log.h"
#include "RuntimeException.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

using namespace std;

#define GPIO1_START_ADDR 0x4804C000
#define GPIO1_END_ADDR 0x4804DFFF
#define GPIO1_SIZE (GPIO1_END_ADDR - GPIO1_START_ADDR)
#define GPIO_OE 0x134
#define GPIO_SETDATAOUT 0x194
#define GPIO_CLEARDATAOUT 0x190

MmapGpio& MmapGpio::instance() {
  static MmapGpio* INSTANCE = NULL;
  if (INSTANCE == NULL) {
    RUN_LOGGING_EXCEPTION({
      INSTANCE = new MmapGpio();
    });
  }
  return *INSTANCE;
}

MmapGpio::MmapGpio() {
  fd = open("/dev/mem", O_RDWR);
  if (fd == -1) {
    throw RuntimeException("unable to open /dev/mem");
  }

  gpioAddr = (uint8_t*)mmap(
      /*addr=*/ 0,
      /*size_t=*/ GPIO1_SIZE,
      /*prot=*/ PROT_READ | PROT_WRITE,
      /*flags=*/ MAP_SHARED,
      fd,
      /*offset=*/ GPIO1_START_ADDR);

  if (gpioAddr == MAP_FAILED) {
    close(fd);
    throw RuntimeException("unable to map GPIO");
  }

  gpioOeAddr = (uint32_t*)(gpioAddr + GPIO_OE);
  gpioSetdataoutAddr = (uint32_t*)(gpioAddr + GPIO_SETDATAOUT);
  gpioCleardataoutAddr = (uint32_t*)(gpioAddr + GPIO_CLEARDATAOUT);

  logDebug("GPIO mapped to %p", gpioAddr);
}

void MmapGpio::test() {
  MmapGpio& m = MmapGpio::instance();

}
