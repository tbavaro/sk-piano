#ifndef __INCLUDED_MMAP_PIN_H
#define __INCLUDED_MMAP_PIN_H

#include "Pin.h"

class MmapPin : public Pin {
  public:
    MmapPin(int number);
    virtual ~MmapPin();

    virtual void close();
    virtual void setMode(Mode mode);
    virtual bool read();
    virtual void write(bool value);
};

#endif
