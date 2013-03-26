#ifndef __INCLUDED_PIANO_H
#define __INCLUDED_PIANO_H

#include <stdint.h>

class Piano {
  public:
    Piano();
    ~Piano();

    void printKeys();
    virtual void scan()=0;

  protected:
    static const int NUM_KEYS = 88;

    uint8_t* const keyValues; 
};

#endif
