#include "LightStrip.h"
#include "LightStripVisualizer.h"

class StackedVisualizer : public LightStripVisualizer {
  public:
    StackedVisualizer(LightStrip& strip);
    ~StackedVisualizer();

    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);
    virtual void reset();

  private:
    Color* pixels;
    int age;
    int num_on;
};
