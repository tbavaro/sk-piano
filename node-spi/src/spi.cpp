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

// NB: in the old environment this was 0, but now it seems to always be 1
static const int EXPECTED_READ_MODE = 1;

static const int FD_NONE = -1;
static const int IOCTL_FAIL = -1;

Spi::Spi(const char* spiDevice, uint32_t _maxSpeedHz)
    : fd(FD_NONE), maxSpeedHz(_maxSpeedHz) {
  if (IS_STUB) {
    return;
  }

  fd = open(spiDevice, O_RDWR);
  if (fd < 0) {
    throw RuntimeException("unable to open SPI device: %s", spiDevice);
  }

  try {
    uint8_t tmp8;
    uint32_t tmp32;

    if (ioctl(fd, SPI_IOC_RD_MODE, &tmp8) == IOCTL_FAIL) {
      throw RuntimeException("unable to get read mode");
    } else if (tmp8 != EXPECTED_READ_MODE) {
      throw RuntimeException("expected read mode %d, got: %d",
          EXPECTED_READ_MODE, tmp8);
    }

    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp8) == IOCTL_FAIL) {
      throw RuntimeException("unable to get bits per word");
    } else if (tmp8 != BITS_PER_WORD) {
      throw RuntimeException("expected %d bits per word, got: %d",
            BITS_PER_WORD, tmp8);
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &maxSpeedHz) == IOCTL_FAIL) {
      throw RuntimeException("unable to set max speed to %d", maxSpeedHz);
    }

    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp32) == IOCTL_FAIL) {
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
  if (fd != FD_NONE) {
    ::close(fd);
    fd = FD_NONE;
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
      printf(" %02x", ((const unsigned char*)buf)[i]);
    }
    printf("\n");
    return;
  }

  if (fd == FD_NONE) {
    throw RuntimeException("SPI instance has been closed");
  }

  int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  if (ret < 1) {
    throw RuntimeException("unable to send SPI message: %d", ret);
  }
}
