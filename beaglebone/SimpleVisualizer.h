#include "LightStrip.h"
#include "LightStripVisualizer.h"

class SimpleVisualizer : public LightStripVisualizer {
  public:
    SimpleVisualizer(LightStrip& strip);
    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
};
