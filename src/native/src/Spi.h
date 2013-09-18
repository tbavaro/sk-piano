#ifndef __INCLUDED_SPI_H
#define __INCLUDED_SPI_H

#include <stdint.h>

class Spi {
  public:
    Spi(const char* spiDevice, uint32_t _maxSpeedHz);
    ~Spi();

    void close();
    void send(void* buf, int numBytes);

private:
    int fd;
    uint32_t maxSpeedHz;
};

#endif
