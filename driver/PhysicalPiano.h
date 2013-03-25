#ifndef __INCLUDED_PHYSICAL_PIANO_H
#define __INCLUDED_PHYSICAL_PIANO_H

#include "Piano.h"

class PhysicalPiano : public Piano {
  public:
    PhysicalPiano();
    
    virtual void scan();

    int fillPressedKeys(uint8_t* keys);

  private:
    int currentOctave;
    int currentNote;
    int currentUnmappedKey;
};

#endif
