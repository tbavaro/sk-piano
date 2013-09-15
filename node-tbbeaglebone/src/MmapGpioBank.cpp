#include "MmapGpioBank.h"

#include "Log.h"
#include "RuntimeException.h"

#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

using namespace std;

static uint32_t GPIO_BANK_START_OFFSETS[] = {
  0x44E07000, // gpio0
  0x4804C000, // gpio1
  0x481AC000  // gpio2
};

static const int GPIO_NUM_BANKS =
    sizeof(GPIO_BANK_START_OFFSETS) / sizeof(GPIO_BANK_START_OFFSETS[0]);

#define GPIO_SIZE 0x1000
#define GPIO_OE 0x134
#define GPIO_DATAIN 0x138
#define GPIO_DATAOUT 0x13C
#define GPIO_SETDATAOUT 0x194
#define GPIO_CLEARDATAOUT 0x190

#define BSET(value, n) (value) = ((value) | (1 << (n)))
#define BCLR(value, n) (value) = ((value) & (0xFFFFFFFF - (1 << (n))))
#define BCHK(value, n) (((value) & (1 << (n))) != 0)

MmapGpioBank& MmapGpioBank::getBank(int bank) {
  static MmapGpioBank* instances[GPIO_NUM_BANKS] = { 0 };

  if (bank < 0 || bank >= GPIO_NUM_BANKS) {
    throw RuntimeException("invalid bank: %d", bank);
  }

  MmapGpioBank* instance = instances[bank];
  if (instance == NULL) {
    instances[bank] = instance =
        new MmapGpioBank(bank, GPIO_BANK_START_OFFSETS[bank]);
  }

  return *instance;
}

static int getOrOpenDevMem() {
  static int fd = -1;

  if (fd == -1) {
    fd = open("/dev/mem", O_RDWR);
    if (fd == -1) {
      throw RuntimeException("unable to open /dev/mem");
    }
  }

  return fd;
}

static void exportPin(int bank, int pinNumber) {
  int id = bank * 32 + pinNumber;

  FILE* f = fopen("/sys/class/gpio/export", "w");
  if (f == NULL) {
    throw RuntimeException("can't open GPIO export file");
  }
  fprintf(f, "%d", id);
  fclose(f);
}

//static void setMuxMode(uint32_t offset)

MmapGpioBank::MmapGpioBank(int bank, uint32_t startOffset) : bank(bank) {
  for (int i = 0; i < 32; ++i) {
    exportPin(bank, i);
  }

  int fd = getOrOpenDevMem();

  logDebug("got fd: %d", fd);

  gpio = (uint8_t*)mmap(
      /*addr=*/ 0,
      /*size_t=*/ GPIO_SIZE,
      /*prot=*/ PROT_READ | PROT_WRITE,
      /*flags=*/ MAP_SHARED,
      fd,
      /*offset=*/ startOffset);

  if (gpio == MAP_FAILED) {
    close(fd);
    throw RuntimeException("unable to map GPIO");
  }

  outputEnable = (uint32_t*)(gpio + GPIO_OE);
  dataIn = (uint32_t*)(gpio + GPIO_DATAIN);
  dataOut = (uint32_t*)(gpio + GPIO_DATAOUT);
  setDataOut = (uint32_t*)(gpio + GPIO_SETDATAOUT);
  clearDataOut = (uint32_t*)(gpio + GPIO_CLEARDATAOUT);

  logDebug("GPIO mapped to %p", gpio);
}

static string bitsToString(uint32_t value) {
  char buf[33] = { 0 };
  for (int i = 0; i < 32; ++i) {
    buf[i] = BCHK(value, 31 - i) ? '1' : '0';
  }
  return buf;
}

void MmapGpioBank::setPinMode(int pinNumber, bool output) {
  logDebug("1 pinmodes: %s", bitsToString(*outputEnable).c_str());
  if (output) {
    BCLR(*outputEnable, pinNumber);
  } else {
    BSET(*outputEnable, pinNumber);
  }
  logDebug("2 pinmodes: %s", bitsToString(*outputEnable).c_str());
}
