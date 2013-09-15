#ifndef __INCLUDED_MMAP_GPIO_H
#define __INCLUDED_MMAP_GPIO_H

#include <stdint.h>

class MmapGpio {
  public:
    static MmapGpio& instance();
    static void test();

  private:
    MmapGpio();

    int fd;
    volatile uint8_t* gpioAddr;
    volatile uint32_t* gpioOeAddr;
    volatile uint32_t* gpioSetdataoutAddr;
    volatile uint32_t* gpioCleardataoutAddr;
};

#endif
