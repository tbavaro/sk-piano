#include "LightStrip.h"
#include "LightStripVisualizer.h"

class DaveeyVisualizer : public LightStripVisualizer {
  public:
    DaveeyVisualizer(LightStrip& strip);
    ~DaveeyVisualizer();

    virtual void onKeyDown(Key key);
    virtual void onKeyUp(Key key);
    virtual void onPassFinished(bool something_changed);

  private:
    int pixelForKey(Key key);

    Color* pixels;
    Color* old_pixels;
    uint32_t prev_frame_time;
};
