#ifndef __INCLUDED_PIANO_H
#define __INCLUDED_PIANO_H

#include "PianoDelegate.h"

class Piano {
  public:
    Piano(PianoDelegate* delegate);
    ~Piano();

    void printKeys();
    void checkOne();

  private:
    PianoDelegate* delegate;
    int current_octave;
    int current_note;
    int current_unmapped_key;
    bool changed_since_last_pass;
    int* key_values; 
};

#endif
