/**
 * node-spi
 * @author tbavaro@gmail.com
 */
#include "spi.h"
#include "runtime_exception.h"

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdexcept>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>

using namespace std;

#ifdef NOT_BEAGLEBONE
static const bool IS_STUB = true;
#else
static const bool IS_STUB = false;
#endif

static const int BITS_PER_WORD = 8;

Spi::Spi(const char* spiDevice, uint32_t _maxSpeedHz)
    : fd(-1), maxSpeedHz(_maxSpeedHz) {
  if (IS_STUB) {
    return;
  }

  fd = open(spiDevice, O_RDWR, 0);
  if (fd < 0) {
    throw RuntimeException("unable to open SPI device: %s", spiDevice);
  }

  try {
    uint8_t tmp8;
    uint32_t tmp32;

    if (ioctl(fd, SPI_IOC_RD_MODE, &tmp8) == -1) {
      throw RuntimeException("unable to get read mode");
    } /* else if (tmp8 != 0) {
      throw RuntimeException("only mode 0 supported; got: %d", tmp8);
    } */

    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp8) == -1) {
      throw RuntimeException("unable to get bits per word");
    } else if (tmp8 != BITS_PER_WORD) {
      throw RuntimeException(
            "only %d bits per word supported; got: %d",
            BITS_PER_WORD, tmp8);
    }

    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &maxSpeedHz);

    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp32) == -1) {
      throw RuntimeException("unable to get max speed");
    } else if (tmp32 != maxSpeedHz) {
      throw RuntimeException(
          "unable to get desired max speed; wanted: %d, got: %d",
          maxSpeedHz, tmp32);
    }
  } catch (const exception& e) {
    this->close();
    throw e;
  }
}

Spi::~Spi() {
  this->close();
}

void Spi::close() {
  if (fd >= 0) {
    ::close(fd);
    fd = -1;
  }
}

void Spi::send(void* buf, int numBytes) {
  uint8_t rx_buf[numBytes];
  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)buf;
  tr.rx_buf = (unsigned long)rx_buf;
  tr.len = numBytes;
  tr.delay_usecs = 5;
  tr.speed_hz = maxSpeedHz;
  tr.bits_per_word = BITS_PER_WORD;

  if (IS_STUB) {
    printf("SEND %d BYTES:", numBytes);
    for (int i = 0; i < numBytes; ++i) {
      printf(" %02x", ((const char*)buf)[i]);
    }
    printf("\n");
    return;
  }

  int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1) {
    throw RuntimeException("unable to send SPI message: %d", ret);
  }
}

