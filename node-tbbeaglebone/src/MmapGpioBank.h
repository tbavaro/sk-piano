#ifndef __INCLUDED_MMAP_GPIO_H
#define __INCLUDED_MMAP_GPIO_H

#include <stdint.h>

class MmapGpioBank {
  public:
    static MmapGpioBank& getBank(int bank);

    void setPinMode(int pinNumber, bool output);
    void writePin(int pinNumber, bool value);
    bool readPin(int pinNumber);

  private:
    MmapGpioBank(int bank, uint32_t startOffset);

    const int bank;
    volatile uint8_t* gpio;
    volatile uint32_t* outputEnable;
    volatile uint32_t* dataIn;
    volatile uint32_t* dataOut;
    volatile uint32_t* setDataOut;
    volatile uint32_t* clearDataOut;
};

inline void MmapGpioBank::writePin(int pinNumber, bool value) {
  *(value ? setDataOut : clearDataOut) = (1 << pinNumber);
}

inline bool MmapGpioBank::readPin(int pinNumber) {
  return *dataIn & (1 << pinNumber);
}

#endif
