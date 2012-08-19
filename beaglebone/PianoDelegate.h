#ifndef __INCLUDED_PIANO_DELEGATE_H
#define __INCLUDED_PIANO_DELEGATE_H

#include "SKTypes.h"

class PianoDelegate {
  public:
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
};

#endif
