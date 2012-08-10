#include "LPD8806.h"
#include "Visualizer.h"

class SimpleVisualizer : public Visualizer {
  public:
    SimpleVisualizer(LPD8806* strip);
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
};
