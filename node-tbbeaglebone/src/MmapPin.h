#ifndef __INCLUDED_MMAP_PIN_H
#define __INCLUDED_MMAP_PIN_H

#include "GpioConfig.h"
#include "MmapGpioBank.h"
#include "Pin.h"

class MmapPin : public Pin {
  public:
    MmapPin(const GpioConfig& pinConfig);
    virtual ~MmapPin();

    virtual void close();
    virtual void setMode(Mode mode);
    virtual bool read();
    virtual void write(bool value);

  private:
    MmapGpioBank& bank;
    const GpioConfig& pinConfig;
};

#endif
