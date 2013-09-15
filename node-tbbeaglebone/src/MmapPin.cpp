#include "MmapPin.h"

#include "Log.h"

MmapPin::MmapPin(const GpioConfig& pinConfig)
    : Pin(pinConfig.exportId),
      bank(MmapGpioBank::getBank(pinConfig.bank)),
      pinConfig(pinConfig) {
}

MmapPin::~MmapPin() {
}

void MmapPin::close() {
  Pin::close();
}

void MmapPin::setMode(Mode mode) {
  logDebug("setting mode: %d", mode);
//  bank.setPinMode(pinConfig.pin, (mode == Pin::OUTPUT));
  Pin::setMode(mode);
}

bool MmapPin::read() {
  return bank.readPin(pinConfig.pin);
}

void MmapPin::write(bool value) {
  bank.writePin(pinConfig.pin, value);
}
