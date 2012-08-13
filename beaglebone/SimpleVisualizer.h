#include "LightStrip.h"
#include "Visualizer.h"

class SimpleVisualizer : public Visualizer {
  public:
    SimpleVisualizer(LightStrip& strip);
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
};
