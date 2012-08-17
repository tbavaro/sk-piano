#ifndef __INCLUDED_PIANO_H
#define __INCLUDED_PIANO_H

#include "PianoDelegate.h"
#include <stdint.h>

class Piano {
  public:
    Piano(PianoDelegate* delegate);
    ~Piano();

    void printKeys();
    virtual bool scan()=0;

  protected:
    static const int NUM_KEYS = 88;

    PianoDelegate* const delegate;
    uint8_t* const key_values; 
};

#endif
