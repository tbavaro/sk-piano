#ifndef __INCLUDED_PIANO_H
#define __INCLUDED_PIANO_H

#include "PianoDelegate.h"
#include <stdint.h>

class Piano {
  public:
    Piano(PianoDelegate* delegate);
    ~Piano();

    void printKeys();
    void checkOne();

  private:
    PianoDelegate* const delegate;
    int current_octave;
    int current_note;
    int current_unmapped_key;
    bool changed_since_last_pass;
    uint8_t* const key_values; 
};

#endif
