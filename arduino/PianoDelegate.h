#ifndef __INCLUDED_PIANO_DELEGATE_H
#define __INCLUDED_PIANO_DELEGATE_H

class PianoDelegate {
  public:
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
    virtual void onPassFinished(bool something_changed);
};

#endif
