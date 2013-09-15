#ifndef __INCLUDED_SLOW_PIN_H
#define __INCLUDED_SLOW_PIN_H

#include "Pin.h"

#include <stdio.h>

class SlowPin : public Pin {
  public:
    SlowPin(const std::string& name, int number);
    virtual ~SlowPin();

    virtual void close();
    virtual void setMode(Mode mode);
    virtual bool read();
    virtual void write(bool value);

  private:
    std::string name;
    std::string gpioPath;
    std::string valueFilename;
    FILE* valueFile;

    FILE* getOrOpenValueFile();
};

#endif
