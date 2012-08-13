#include "LightStrip.h"
#include "Visualizer.h"

class SimpleVisualizer : public Visualizer {
  public:
    SimpleVisualizer(LightStrip& strip);
    ~SimpleVisualizer();

    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);

  private:
    int* pixel_counts;
    Color colorForCount(int count);
    int pixelForKey(Key key);
};
