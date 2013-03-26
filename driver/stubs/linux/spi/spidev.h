#ifndef __INCLUDED_SPIDEV_H
#define __INCLUDED_SPIDEV_H

#include <stdint.h>

#define SPI_IOC_RD_MODE 0
#define SPI_IOC_RD_BITS_PER_WORD 0
#define SPI_IOC_WR_MAX_SPEED_HZ 0
#define SPI_IOC_RD_MAX_SPEED_HZ 0
#define SPI_IOC_MESSAGE(n) 0

struct spi_ioc_transfer {
  unsigned long tx_buf;
  unsigned long rx_buf;
  int len;
  int delay_usecs;
  uint32_t speed_hz;
  int bits_per_word;
};

#endif
