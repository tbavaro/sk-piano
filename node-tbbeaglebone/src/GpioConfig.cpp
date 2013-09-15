#include "GpioConfig.h"

#include "RuntimeException.h"

#include <stdio.h>

static const GpioConfig* USER_LEDS[] = {
  new GpioConfig(1, 21, "usr0"),
  new GpioConfig(1, 22, "usr1"),
  new GpioConfig(1, 23, "usr2"),
  new GpioConfig(1, 24, "usr3")
};
static const int NUM_USER_LEDS = sizeof(USER_LEDS) / sizeof(USER_LEDS[0]);

static const GpioConfig* HEADER_8[] = {
  NULL,
  
  // 1:
  NULL,
  NULL,
  new GpioConfig(1,  6, "gpmc_ad6"),
  new GpioConfig(1,  7, "gpmc_ad7"),
  new GpioConfig(1,  2, "gpmc_ad2"),

  // 6:
  NULL, //  new GpioConfig(0, 0, 35, "gpmc_ad3"), // something's wrong here, maybe reserved?
  new GpioConfig(2,  2, "gpmc_advn_ale"),
  new GpioConfig(2,  3, "gpmc_oen_ren"),
  new GpioConfig(2,  5, "gpmc_ben0_cle"),
  new GpioConfig(2,  4, "gpmc_wen"),

  // 11:
  new GpioConfig(1, 13, "gpmc_ad13"),
  new GpioConfig(1, 12, "gpmc_ad12"),
  new GpioConfig(0, 23, "gpmc_ad9"),
  new GpioConfig(0, 26, "gpmc_ad10"),
  new GpioConfig(1, 15, "gpmc_ad15"),

  // 16:
  new GpioConfig(1, 14, "gpmc_ad14"),
  new GpioConfig(0, 27, "gpmc_ad11"),
  new GpioConfig(2,  1, "gpmc_clk"),
  new GpioConfig(0, 22, "gpmc_ad8"),
  new GpioConfig(1, 31, "gpmc_csn2"),

  // 21:
  new GpioConfig(1, 30, "gpmc_csn1"),
  new GpioConfig(1,  5, "gpmc_ad5"),
  new GpioConfig(1,  4, "gpmc_ad4"),
  new GpioConfig(1,  1, "gpmc_ad1"),
  new GpioConfig(1,  0, "gpmc_ad0"),

  // 26:
  new GpioConfig(1, 29, "gpmc_csn0")
};
static const int HEADER_8_SIZE = sizeof(HEADER_8) / sizeof(HEADER_8[0]);

const GpioConfig* GpioConfig::userLed(int i) {
  if (i < 0 || i >= NUM_USER_LEDS) {
    return NULL;
  } else {
    return USER_LEDS[i];
  }
}

const GpioConfig* GpioConfig::lookup(int headerNum, int pin) {
  const GpioConfig** header = NULL;
  int headerSize = 0;

  switch (headerNum) {
    case 8:
      header = HEADER_8;
      headerSize = HEADER_8_SIZE;
      break;

    default:
      break;
  }

  if (pin < 0 || pin >= headerSize) {
    return NULL;
  } else {
    return header[pin];
  }
}

GpioConfig::GpioConfig(int bank, int pin, const char* name)
    : bank(bank),
      pin(pin),
      exportId(bank * 32 + pin),
      name(name) {
}
