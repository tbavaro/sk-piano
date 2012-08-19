#ifndef __INCLUDED_PHYSICAL_PIANO_H
#define __INCLUDED_PHYSICAL_PIANO_H

#include "Piano.h"

class PhysicalPiano : public Piano {
  public:
    PhysicalPiano(PianoDelegate* delegate);
    
    virtual bool scan();

  private:
    int current_octave;
    int current_note;
    int current_unmapped_key;
    bool changed_since_last_pass;
};

#endif
